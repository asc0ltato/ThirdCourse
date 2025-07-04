using CentralServer.Models;
using System.Text.Json;

namespace CentralServer.Services
{
    public class TelemetryService : ITelemetryService
    {
        // Фабрика для создания HTTP-клиентов - позволяет повторно использовать соединения
        private readonly IHttpClientFactory _httpClientFactory;
        private readonly ILogger<TelemetryService> _logger;

        public TelemetryService(IHttpClientFactory httpClientFactory, ILogger<TelemetryService> logger)
        {
            _httpClientFactory = httpClientFactory ?? throw new ArgumentNullException(nameof(httpClientFactory));
            _logger = logger ?? throw new ArgumentNullException(nameof(logger));
        }

        // Асинхронное получение телеметрических данных с удаленного источника по URL
        public async Task<List<TelemetryData>> GetTelemetryDataAsync(string sourceUrl)
        {
            try
            {
                _logger.LogInformation($"[GetTelemetryDataAsync] Получение телеметрических данных из {sourceUrl}");
                // Получение клиента из фабрики
                var httpClient = _httpClientFactory.CreateClient();
                // HTTP GET запрос
                var response = await httpClient.GetAsync(sourceUrl);

                // Если OK 200
                if (response.IsSuccessStatusCode)
                {
                    // Чтение тела ответа в виде строки
                    var content = await response.Content.ReadAsStringAsync();
                    var options = new JsonSerializerOptions { PropertyNameCaseInsensitive = true };
                    // Десериализация JSON в список TelemetryData
                    var telemetryData = JsonSerializer.Deserialize<List<TelemetryData>>(content, options);

                    if (telemetryData != null)
                    {
                        _logger.LogInformation($"[SUCCESS] Успешно получены {telemetryData.Count} записей телеметрии из {sourceUrl}");
                        return telemetryData;
                    }
                    else
                    {
                        _logger.LogError($"[ERROR] Не удалось десериализовать данные телеметрии из {sourceUrl}. Содержимое: {content}");
                        return new List<TelemetryData>();
                    }
                }
                else
                {
                    _logger.LogError($"[ERROR] Не удалось получить телеметрические данные из {sourceUrl}. Код состояния: {response.StatusCode}");
                    return new List<TelemetryData>();
                }
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, $"[ERROR] Произошла ошибка при получении телеметрических данных из {sourceUrl}");
                return new List<TelemetryData>();
            }
        }

        // Проверка доступности источника по URL
        public async Task<bool> IsSourceAvailableAsync(string sourceUrl)
        {
            try
            {
                var httpClient = _httpClientFactory.CreateClient();
                var response = await httpClient.GetAsync(sourceUrl);
                // Если 200 - доступен
                return response.IsSuccessStatusCode;
            }
            catch (Exception)
            {
                return false;
            }
        }
    }
}