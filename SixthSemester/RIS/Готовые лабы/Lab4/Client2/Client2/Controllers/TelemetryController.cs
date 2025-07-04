using Microsoft.AspNetCore.Mvc;
using Client2.Models;

namespace Client2.Controllers
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
            _logger.LogInformation("[GetTelemetry] Получен запрос от источника 2");

            var telemetryDataList = new List<TelemetryData>();

            for (int i = 0; i < 5; i++)
            {
                var telemetryData = new TelemetryData
                {
                    SourceId = "Source2",
                    Timestamp = DateTime.UtcNow,
                    MeasurementType = i == 0 ? "BatteryLevel" : (i == 1 ? "SignalStrength" : "GPSQuality"),
                    Unit = i == 0 ? "%" : (i == 1 ? "dBm" : "Stars")
                };

                // С вероятностью 20% создание записи с ошибкой
                if (_random.Next(10) < 2)
                {
                    telemetryData.Status = "Error";
                    telemetryData.ErrorMessage = "Возникла неисправность";
                    _logger.LogError("Произошла ошибка при считывании данных датчика в Source2");
                }
                else
                {
                    telemetryData.Value = _random.Next(0, 100); // Заряд батареи
                }

                telemetryDataList.Add(telemetryData);
            }

            return Ok(telemetryDataList);
        }
    }
}