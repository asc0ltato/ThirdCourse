using CentralServer.Models;

namespace CentralServer.Services
{
    // Запуск вместе с приложением (собирает данные в фоне каждые 10 сек)
    public class TelemetryBackgroundService : BackgroundService
    {
        private readonly IServiceProvider _serviceProvider;
        private readonly IConfiguration _configuration;
        private readonly ILogger<TelemetryBackgroundService> _logger;

        // Хранилище телеметрии
        private static readonly List<TelemetryData> _telemetryDataStore = new List<TelemetryData>();

        public TelemetryBackgroundService(IServiceProvider serviceProvider, IConfiguration configuration, ILogger<TelemetryBackgroundService> logger)
        {
            _serviceProvider = serviceProvider ?? throw new ArgumentNullException(nameof(serviceProvider));
            _configuration = configuration ?? throw new ArgumentNullException(nameof(configuration));
            _logger = logger ?? throw new ArgumentNullException(nameof(logger));
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            _logger.LogInformation("[TelemetryBackgroundService] запущен");

            while (!stoppingToken.IsCancellationRequested)
            {
                try
                {
                    // Scope для получения зависимостей (ITelemetryService)
                    using (var scope = _serviceProvider.CreateScope())
                    {
                        var telemetryService = scope.ServiceProvider.GetRequiredService<ITelemetryService>();
                        // Загрузка списка URL из appsettings.json
                        List<string> sourceUrls = new List<string>();
                        _configuration.GetSection("TelemetrySources").Bind(sourceUrls);

                        foreach (var sourceUrl in sourceUrls)
                        {
                            // Если источник доступен
                            if (await telemetryService.IsSourceAvailableAsync(sourceUrl))
                            {
                                var telemetryData = await telemetryService.GetTelemetryDataAsync(sourceUrl);
                                if (telemetryData != null)
                                {
                                    // Отброс данных со статусом "Error"
                                    var validTelemetryData = telemetryData.Where(data => data.Status != "Error").ToList();

                                    // Добавление валидных данных в хранилище с lock
                                    lock (_telemetryDataStore)
                                    {
                                        _telemetryDataStore.AddRange(validTelemetryData);
                                    }
                                }
                            }
                            else
                            {
                                _logger.LogWarning($"[WARNING] Источник [{sourceUrl}] недоступен");
                            }

                        }
                    }
                }
                catch (Exception ex)
                {
                    _logger.LogError(ex, "[ERROR] Во время сбора телеметрических данных произошла ошибка");
                }

                // Опрос каждые 10 секунд
                await Task.Delay(TimeSpan.FromSeconds(10), stoppingToken);
            }

            _logger.LogInformation("[TelemetryBackgroundService] остановлен");
        }

        // Получение всех сохраненных данных (копия списка)
        public static List<TelemetryData> GetAllTelemetryData()
        {
            lock (_telemetryDataStore)
            {
                return new List<TelemetryData>(_telemetryDataStore);
            }
        }
    }
}