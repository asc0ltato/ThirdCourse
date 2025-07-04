using CentralServer.Data;
using CentralServer.Models;
using Microsoft.EntityFrameworkCore;
using System.Globalization;

namespace CentralServer.Services
{
    //5. Вытягивать из ТО в ГО
    public class ReplicationService : BackgroundService
    {
        private readonly ILogger<ReplicationService> _logger;
        private readonly IServiceScopeFactory _scopeFactory;
        private readonly IConfiguration _configuration;
        private readonly IHttpClientFactory _httpClientFactory;
        private readonly TimeSpan _interval;
        private readonly string[] _sourceIdentifiers = { "Source1", "Source2" };

        public ReplicationService(ILogger<ReplicationService> logger,
                                  IServiceScopeFactory scopeFactory,
                                  IConfiguration configuration,
                                  IHttpClientFactory httpClientFactory)
        {
            _logger = logger;
            _scopeFactory = scopeFactory;
            _configuration = configuration;
            _httpClientFactory = httpClientFactory;
            _interval = TimeSpan.FromSeconds(
                _configuration.GetValue<int>("ReplicationSettings:IntervalSeconds", 15));
            _logger.LogInformation("Интервал репликации установлен на {Interval}", _interval);
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            _logger.LogInformation("[{ServiceName}] запущен", nameof(ReplicationService));
            try { await Task.Delay(TimeSpan.FromSeconds(5), stoppingToken); }
            catch (OperationCanceledException) { return; }

            while (!stoppingToken.IsCancellationRequested)
            {
                _logger.LogInformation("Запуск цикла репликации в {Time}", DateTimeOffset.Now);
                try
                {
                    using (var scope = _scopeFactory.CreateScope())
                    {
                        var centralDbContext = scope.ServiceProvider.GetRequiredService<CentralDBContext>();

                        foreach (var sourceId in _sourceIdentifiers)
                        {
                            await ReplicateFromSourceAsync(sourceId, centralDbContext, stoppingToken);
                        }
                    }
                }
                catch (OperationCanceledException)
                {
                    _logger.LogInformation("[{ServiceName}] цикл отменен во время обработки", nameof(ReplicationService));
                    break;
                }
                catch (Exception ex)
                {
                    _logger.LogError(ex, "[ERROR] Во время основного цикла репликации произошла ошибка");
                }

                _logger.LogInformation("[SUCCESS] Цикл репликации завершен. В ожидании {Interval}", _interval);
                try { await Task.Delay(_interval, stoppingToken); }
                catch (OperationCanceledException) { break; }
            }
            _logger.LogInformation("[{ServiceName}] остановлен", nameof(ReplicationService));
        }

        private async Task ReplicateFromSourceAsync(string sourceIdentifier,
                                            CentralDBContext centralDbContext,
                                            CancellationToken stoppingToken)
        {
            try
            {
                _logger.LogInformation("[SUCCESS] Репликация данных для источника: {SourceId}", sourceIdentifier);
                DateTime lastReplicatedTimestamp = await centralDbContext.TelemetryData
                   .Where(t => t.SourceIdentifier == sourceIdentifier)
                   .OrderByDescending(t => t.Timestamp)
                   .Select(t => t.Timestamp)
                   .FirstOrDefaultAsync(stoppingToken);
                _logger.LogDebug("Последняя реплицированная временная метка для {SourceId} в {Timestamp}", sourceIdentifier, lastReplicatedTimestamp);

                string? baseUrl = _configuration[$"ReplicationSettings:SourceServices:{sourceIdentifier}:BaseUrl"];
                string? path = _configuration[$"ReplicationSettings:SourceServices:{sourceIdentifier}:TelemetryPath"];
                if (string.IsNullOrEmpty(baseUrl) || string.IsNullOrEmpty(path)) { return; }

                string formattedTimestamp = lastReplicatedTimestamp.ToString("O", CultureInfo.InvariantCulture);
                string requestUrl = $"{baseUrl.TrimEnd('/')}/{path.TrimStart('/')}?since={Uri.EscapeDataString(formattedTimestamp)}";
                _logger.LogDebug("Запрашиваемые данные у URL: {Url}", requestUrl);

                List<TelemetryData> newTelemetryDataForDb = null;

                HttpClient client = _httpClientFactory.CreateClient();
                HttpResponseMessage response = await client.GetAsync(requestUrl, stoppingToken);

                if (response.IsSuccessStatusCode)
                {
                    var receivedData = await response.Content.ReadFromJsonAsync<List<TelemetryData>>(cancellationToken: stoppingToken);
                    _logger.LogInformation("[SUCCESS] Получено {Count} записей из API {SourceId}", receivedData?.Count ?? 0, sourceIdentifier);

                    if (receivedData != null && receivedData.Any())
                    {
                        newTelemetryDataForDb = receivedData.Select(sourceData => new TelemetryData
                        {
                            SourceIdentifier = sourceData.SourceIdentifier,
                            ObjectId = sourceData.ObjectId,
                            Timestamp = sourceData.Timestamp,
                            Value = sourceData.Value
                        }).ToList();
                    }
                }
                else
                {
                    string errorContent = await response.Content.ReadAsStringAsync(stoppingToken);
                    _logger.LogWarning("[WARNING] Не удалось получить данные из API {SourceId}. Статус: {StatusCode} {ReasonPhrase}. URL: {Url}. Содержание: {ErrorContent}",
                                      sourceIdentifier, (int)response.StatusCode, response.ReasonPhrase, requestUrl, errorContent);
                    return;
                }

                if (newTelemetryDataForDb != null && newTelemetryDataForDb.Any())
                {
                    await ProcessNewData(newTelemetryDataForDb, centralDbContext, sourceIdentifier, stoppingToken);
                }
                else if (newTelemetryDataForDb != null)
                {
                    _logger.LogInformation("[SUCCESS] После обработки ответа API для {SourceId} не найдено новых записей", sourceIdentifier);
                }

            }
            catch (HttpRequestException ex)
            {
                _logger.LogWarning("[WARNING] Источник {SourceId}, похоже, недоступен. Не удалось подключиться или установить связь. Сообщение: {ErrorMessage}",
                                 sourceIdentifier, ex.Message);
            }
            catch (OperationCanceledException) when (stoppingToken.IsCancellationRequested)
            {
                _logger.LogInformation("[WARNING] Репликация для {SourceId} отменена", sourceIdentifier);
            }
            catch (System.Text.Json.JsonException jsonEx)
            {
                _logger.LogError(jsonEx, "[ERROR] Не удалось десериализовать JSON-ответ из {SourceID}", sourceIdentifier);
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "[ERROR] Произошла непредвиденная ошибка при репликации данных для источника: {SourceId}", sourceIdentifier);
            }
        }

        private async Task ProcessNewData(List<TelemetryData> newData,
                                          CentralDBContext centralDbContext,
                                          string sourceId,
                                          CancellationToken stoppingToken)
        {
            if (newData.Any())
            {
                _logger.LogInformation("[SUCCESS] Найдено {Count} новых записей из API для {SourceId} для вставки/удаления", newData.Count, sourceId);
                await centralDbContext.TelemetryData.AddRangeAsync(newData, stoppingToken);
                int recordsSaved = await centralDbContext.SaveChangesAsync(stoppingToken);
                _logger.LogInformation("[SUCCESS] Успешно сохранены новые записи {Count} из {SourceId} в центральной базе данных", recordsSaved, sourceId);
            }
        }
    }
}