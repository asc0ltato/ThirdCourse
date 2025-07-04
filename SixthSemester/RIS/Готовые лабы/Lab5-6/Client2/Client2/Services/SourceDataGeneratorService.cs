using Client2.Data;
using Client2.Models;

namespace Client2.Services
{
    public class SourceDataGeneratorService : BackgroundService
    {
        private readonly ILogger<SourceDataGeneratorService> _logger;
        private readonly IServiceScopeFactory _scopeFactory; // Фабрика скоупов для получения DbContext внутри фонового сервиса
        private readonly Random _random = new Random();
        private const string SourceIdentifier = "Source2"; // Идентификатор источника
        private const int NumberOfObjects = 10; // Количество объектов
        private const int DelaySeconds = 20; // Задержка между генерациями данных

        public SourceDataGeneratorService(ILogger<SourceDataGeneratorService> logger, IServiceScopeFactory scopeFactory)
        {
            _logger = logger;
            _scopeFactory = scopeFactory;
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            _logger.LogInformation("[{ServiceName}] запущен", nameof(SourceDataGeneratorService));

            while (!stoppingToken.IsCancellationRequested)
            {
                try
                {
                    using (var scope = _scopeFactory.CreateScope())
                    {
                        // Получение контекста бд из DI
                        var dbContext = scope.ServiceProvider.GetRequiredService<Source2DBContext>();

                        var timestamp = DateTime.UtcNow; // Текущее время для всех записей

                        _logger.LogInformation("[SUCCESS] Генерация данных для {Source} в {Timestamp}", SourceIdentifier, timestamp);

                        for (int i = 1; i <= NumberOfObjects; i++)
                        {
                            var telemetryEntry = new TelemetryData
                            {
                                SourceIdentifier = SourceIdentifier,
                                ObjectId = $"Meter_{i:D2}",
                                Timestamp = timestamp,
                                Value = _random.NextDouble() * (5 - 1) + 1 // Случайное значение от 1 до 5
                            };

                            // Асинхронное добавление в базу
                            await dbContext.TelemetryData.AddAsync(telemetryEntry, stoppingToken);
                        }

                        // Сохранение всех добавленных записей
                        int recordsSaved = await dbContext.SaveChangesAsync(stoppingToken);
                        _logger.LogInformation("[SUCCESS] Сохранено {Count} новых записей телеметрии для {Source}", recordsSaved, SourceIdentifier);
                    }
                }
                catch (OperationCanceledException)
                {
                    _logger.LogInformation("[{ServiceName}] остановлен", nameof(SourceDataGeneratorService));
                }
                catch (Exception ex)
                {
                    _logger.LogError(ex, "[ERROR] Произошла ошибка в {ServiceName}.", nameof(SourceDataGeneratorService));
                    await Task.Delay(TimeSpan.FromSeconds(30), stoppingToken); // 30 секунд перед повтором чил, чтобы не спамить лог
                }

                // Ждем перед следующей генерацией 20 сек
                await Task.Delay(TimeSpan.FromSeconds(DelaySeconds), stoppingToken);
            }
        }
    }
}