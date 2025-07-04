using Microsoft.EntityFrameworkCore;
using CentralServer.Data;
using CentralServer.Models;

namespace CentralServer.Services
{
    //6. Выталкивание из ГО в ТО
    public class PushReplicationService : BackgroundService
    {
        private readonly ILogger<PushReplicationService> _logger;
        private readonly IServiceScopeFactory _scopeFactory;
        private readonly IConfiguration _configuration;
        private readonly IHttpClientFactory _httpClientFactory;
        private readonly TimeSpan _interval; // Интервал между репликациями
        private DateTime _lastSuccessfullyPushedTimestamp = DateTime.MinValue; // Метка времени самой последней успешно отправленной записи

        public PushReplicationService(ILogger<PushReplicationService> logger,
                                  IServiceScopeFactory scopeFactory,
                                  IConfiguration configuration,
                                  IHttpClientFactory httpClientFactory)
        {
            _logger = logger;
            _scopeFactory = scopeFactory;
            _configuration = configuration;
            _httpClientFactory = httpClientFactory;
            _interval = TimeSpan.FromSeconds(
                _configuration.GetValue<int>("PushReplicationSettings:IntervalSeconds", 15));
            _logger.LogInformation("[SUCCESS] Интервал установлен равным {Interval}", _interval);
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            _logger.LogInformation("[{PushService}] запущен", nameof(PushReplicationService));

            // Небольшая задержка перед началом (чтобы все клиенты успели подняться)
            try
            {
                await Task.Delay(TimeSpan.FromSeconds(15), stoppingToken);
            }
            catch (OperationCanceledException) { _logger.LogInformation("[ERROR] Отменен во время первоначальной задержки"); return; }


            while (!stoppingToken.IsCancellationRequested)
            {
                _logger.LogInformation("[SUCCESS] Цикл выталкивания начался в {Time}", DateTimeOffset.Now);
                DateTime currentCycleMaxTimestamp = DateTime.MinValue;

                try
                {
                    List<TelemetryData> newDataToSend;

                    using (var scope = _scopeFactory.CreateScope())
                    {
                        // Создание нового скоупа для доступа к зависимостям
                        var dbContext = scope.ServiceProvider.GetRequiredService<CentralDBContext>();

                        // Получение новых записей телеметрии, которые появились после последней успешной отправки
                        newDataToSend = await dbContext.TelemetryData
                            .Where(t => t.Timestamp > _lastSuccessfullyPushedTimestamp)
                            .OrderBy(t => t.Timestamp)
                            .ToListAsync(stoppingToken);

                        if (newDataToSend.Any())
                        {
                            _logger.LogInformation("[SUCCESS] Найдено {Count} новых записей в центральной базе данных для отправки (начиная с {LastPushTime})",
                                newDataToSend.Count, _lastSuccessfullyPushedTimestamp);

                            // Запоминаем максимальное время из новых записей
                            currentCycleMaxTimestamp = newDataToSend.Max(t => t.Timestamp);
                        }
                        else
                        {
                            _logger.LogWarning("[WARNING] В центральной базе данных не найдено новых записей для отправки (начиная с  {LastPushTime})", _lastSuccessfullyPushedTimestamp);
                            await Task.Delay(_interval, stoppingToken);
                            continue;
                        }
                    }

                    // Отправляем найденные данные каждому клиенту
                    var targets = _configuration.GetSection("PushTargets").GetChildren().ToList();
                    if (!targets.Any())
                    {
                        _logger.LogWarning("[WARNING] В appsettings.json не настроены push-цели");
                        await Task.Delay(_interval, stoppingToken);
                        continue;
                    }

                    // Флаг, указывающий, успешно ли прошла отправка на все цели
                    bool allPushesSuccessful = true;
                    var httpClient = _httpClientFactory.CreateClient();

                    // Отправка данных на каждый целевой сервер
                    foreach (var target in targets)
                    {
                        string targetName = target.Key;
                        string targetUrl = target.Value;

                        if (string.IsNullOrEmpty(targetUrl))
                        {
                            _logger.LogWarning("[WARNING] Push-адрес назначения для '{TargetName}' пуст, пропускается", targetName);
                            continue;
                        }

                        // Отправка данных
                        bool pushSuccess = await PushDataToTargetAsync(httpClient, targetName, targetUrl, newDataToSend, stoppingToken);
                        if (!pushSuccess)
                        {
                            allPushesSuccessful = false;
                            _logger.LogWarning("[WARNING] Не удалось выполнить переход к {TargetName}. Данные будут удалены", targetName);
                        }
                    }

                    // Обновление метки времени при успехе
                    if (allPushesSuccessful && newDataToSend.Any())
                    {
                        _lastSuccessfullyPushedTimestamp = currentCycleMaxTimestamp;
                        _logger.LogInformation("[SUCCESS] Данные успешно отправлены во все целевые объекты. Временная метка последней отправки обновлена до {Timestamp}", _lastSuccessfullyPushedTimestamp);
                    }
                    else if (!allPushesSuccessful && newDataToSend.Any())
                    {
                        _logger.LogWarning("[WARNING] Не все запросы выполнены успешно. Временная метка последнего запроса не обновлена");
                    }
                }
                catch (OperationCanceledException)
                {
                    _logger.LogInformation("[{Push Service}] Требуется остановка", nameof(PushReplicationService));
                    break;
                }
                catch (Exception ex)
                {
                    _logger.LogError(ex, "[ERROR] Необработанная ошибка во время цикла push-запроса");
                    await Task.Delay(TimeSpan.FromSeconds(30), stoppingToken);
                }

                _logger.LogInformation("[SUCCESS] Цикл push завершен. Ожидание {Interval}.", _interval);
                try
                {
                    await Task.Delay(_interval, stoppingToken);
                }
                catch (OperationCanceledException)
                {
                    _logger.LogInformation("[{PushService}] Остановка во время задержки", nameof(PushReplicationService));
                    break;
                }
            }

            _logger.LogInformation("[{PushService}] остановлен", nameof(PushReplicationService));
        }

        // На один клиент
        private async Task<bool> PushDataToTargetAsync(HttpClient httpClient, string targetName, string targetUrl, List<TelemetryData> data, CancellationToken stoppingToken)
        {
            _logger.LogInformation("[PushDataToTargetAsync] Попытка push {Count} записей для {TargetName} ({TargetUrl})", data.Count, targetName, targetUrl);
            try
            {
                // Отправка данных в формате JSON методом POST
                HttpResponseMessage response = await httpClient.PostAsJsonAsync(targetUrl, data, stoppingToken);

                if (response.IsSuccessStatusCode)
                {
                    _logger.LogInformation("[SUCCESS] Успешно перенесенные данные в {TargetName}. Статус: {StatusCode}", targetName, response.StatusCode);
                    return true;
                }
                else
                {
                    string errorContent = await response.Content.ReadAsStringAsync(stoppingToken);
                    _logger.LogError("[ERROR] Не удалось отправить данные в {TargetName}. Статус: {StatusCode} ({Reason}). URL: {TargetUrl}. Ответ: {ErrorContent}",
                                     targetName, response.StatusCode, response.ReasonPhrase, targetUrl, errorContent);
                    return false;
                }
            }

            catch (HttpRequestException ex)
            {
                _logger.LogError(ex, "[ERROR] Ошибка HTTP-запроса при отправке в {TargetName} ({TargetUrl}). Проверьте подключение/URL", targetName, targetUrl);
                return false;
            }
            catch (OperationCanceledException)
            {
                _logger.LogWarning("[WARNING] Операция отправки в {TargetName} была отменена", targetName);
                return false;
            }

            catch (Exception ex)
            {
                _logger.LogError(ex, "[ERROR] Непредвиденная ошибка при передаче данных в {TargetName} ({TargetUrl})", targetName, targetUrl);
                return false;
            }
        }
    }
}