using Client2.Controllers;

namespace Client2.Services
{
    public class TimeSyncBackgroundService : BackgroundService
    {
        private readonly ILogger<TimeSyncBackgroundService> _logger;
        private readonly IHttpClientFactory _httpClientFactory;

        public TimeSyncBackgroundService(ILogger<TimeSyncBackgroundService> logger, IHttpClientFactory httpClientFactory)
        {
            _logger = logger;
            _httpClientFactory = httpClientFactory;
        }

        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            _logger.LogInformation("[TimeSyncBackgroundService] запущен");

            var client = _httpClientFactory.CreateClient();
            string syncUrl = "http://localhost:5223/CentralTelemetry/sync-time";
            int Tc = 10000;
            bool firstSync = true;

            while (!stoppingToken.IsCancellationRequested)
            {
                long curvalue = new DateTimeOffset(DateTime.Now).ToUnixTimeMilliseconds();

                var request = new
                {
                    Cmd = "SINC",
                    CurValue = curvalue
                };

                try
                {
                    var response = await client.PostAsJsonAsync(syncUrl, request, stoppingToken);
                    if (response.IsSuccessStatusCode)
                    {
                        var result = await response.Content.ReadFromJsonAsync<SetSinchroResponse>(cancellationToken: stoppingToken);
                        long correction = result?.Correction ?? 0;
                        long corrected = curvalue + correction;

                        if (!firstSync)
                        {
                            _logger.LogInformation($"[SyncTime] CurValue = {curvalue}, Correction = {correction}, Corrected = {corrected}");
                        }
                        else
                        {
                            firstSync = false;
                        }
                    }
                    else
                    {
                        _logger.LogWarning($"[SyncTime] Ошибка HTTP: {response.StatusCode}");
                    }
                }
                catch (Exception ex)
                {
                    _logger.LogError(ex, "[SyncTime] Ошибка синхронизации");
                }

                await Task.Delay(Tc, stoppingToken);
            }

            _logger.LogInformation("[TimeSyncBackgroundService] остановлен");
        }
    }
}