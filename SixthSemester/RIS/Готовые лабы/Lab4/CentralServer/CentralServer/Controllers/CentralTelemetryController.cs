using CentralServer.Models;
using CentralServer.Services;
using Microsoft.AspNetCore.Mvc;

namespace CentralServer.Controllers
{
    public class GetSinchroRequest
    {
        public string Cmd { get; set; } = "SINC";
        public long CurValue { get; set; } // Локальное клиентское время
    }

    public class SetSinchroResponse
    {
        public string Cmd { get; set; } = "SINCRO";
        public long Correction { get; set; } // Разница между сервером и клиентом
    }

    [ApiController]
    [Route("[controller]")]
    public class CentralTelemetryController : ControllerBase
    {
        private readonly ITelemetryService _telemetryService;
        private readonly IConfiguration _configuration;
        private readonly ILogger<CentralTelemetryController> _logger;

        public CentralTelemetryController(ITelemetryService telemetryService, IConfiguration configuration, ILogger<CentralTelemetryController> logger)
        {
            _telemetryService = telemetryService ?? throw new ArgumentNullException(nameof(telemetryService));
            _configuration = configuration ?? throw new ArgumentNullException(nameof(configuration));
            _logger = logger ?? throw new ArgumentNullException(nameof(logger));
        }

        // Обработка запроса для получения всей телеметрии из фонового сервиса
        [HttpGet("all")]
        public IActionResult GetAllTelemetry()
        {
            _logger.LogInformation("[GetAllTelemetry] получен запрос");
            var allData = TelemetryBackgroundService.GetAllTelemetryData();
            return Ok(allData);
        }

        // Проверка статуса доступности всех источников, указанных в appsettings.json
        [HttpGet("sources/status")]
        public async Task<IActionResult> GetSourcesStatus()
        {
            _logger.LogInformation("[GetSourcesStatus] получен запрос");

            // Загрузка списка URL из appsettings.json
            List<string> sourceUrls = new List<string>();
            _configuration.GetSection("TelemetrySources").Bind(sourceUrls);

            var sourcesStatus = new Dictionary<string, bool>();

            // Асинхронная проверка каждого источника на доступность через сервис телеметрии
            foreach (var sourceUrl in sourceUrls)
            {
                sourcesStatus[sourceUrl] = await _telemetryService.IsSourceAvailableAsync(sourceUrl);
            }

            return Ok(sourcesStatus);
        }

        // Обработка Post запроса синхронизации времени
        [HttpPost("sync-time")]
        public IActionResult GetTimeCorrection([FromBody] GetSinchroRequest request)
        {
            long serverTicks = new DateTimeOffset(DateTime.Now).ToUnixTimeMilliseconds();
            long correction = serverTicks - request.CurValue;

            return Ok(new SetSinchroResponse
            {
                Cmd = "SINCRO",
                Correction = correction
            });
        }
    }
}