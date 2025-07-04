using Microsoft.AspNetCore.Mvc;
using Client1.Models;

namespace Client1.Controllers
{
    public class SetSinchroResponse
    {
        public string Cmd { get; set; }
        public long Correction { get; set; }
    }

    [ApiController]
    [Route("[controller]")]
    public class TelemetryController : ControllerBase
    {
        private readonly ILogger<TelemetryController> _logger;
        private readonly Random _random = new Random();

        public TelemetryController(ILogger<TelemetryController> logger)
        {
            _logger = logger;
        }

        // Возвращение случайно сгенерированных данных телеметрии
        [HttpGet]
        public IActionResult GetTelemetry()
        {
            _logger.LogInformation("[GetTelemetry] Получен запрос от источника 1");

            var telemetryDataList = new List<TelemetryData>();

            for (int i = 0; i < 5; i++)
            {
                var telemetryData = new TelemetryData
                {
                    SourceId = "Source1",
                    Timestamp = DateTime.UtcNow,
                    MeasurementType = i == 0 ? "Temperature" : (i == 1 ? "Pressure" : "Voltage"),
                    Unit = i == 0 ? "°C" : (i == 1 ? "kPa" : "V")
                };

                // С вероятностью 20% создание записи с ошибкой
                if (_random.Next(10) < 2)
                {
                    telemetryData.Status = "Error";
                    telemetryData.ErrorMessage = "Не удалось считать данные";
                    _logger.LogError("Произошла ошибка при считывании данных датчика в Source1");
                }
                else
                {
                    telemetryData.Value = _random.Next(0, 25); // Температура
                }

                telemetryDataList.Add(telemetryData);
            }

            return Ok(telemetryDataList);
        }
    }
}