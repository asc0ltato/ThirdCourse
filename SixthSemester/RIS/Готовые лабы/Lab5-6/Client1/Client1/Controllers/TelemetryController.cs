using Microsoft.AspNetCore.Mvc;
using Client1.Models;
using Microsoft.EntityFrameworkCore;
using Client1.Data;

namespace Client1.Controllers
{
    //5. Вытягивать из ТО в ГО
    [Route("api/[controller]")]
    [ApiController]
    public class TelemetryController : ControllerBase
    {
        private readonly ILogger<TelemetryController> _logger;
        private readonly Source1DBContext _context;

        public TelemetryController(Source1DBContext context, ILogger<TelemetryController> logger)
        {
            _context = context;
            _logger = logger;
        }

        [HttpGet]
        public async Task<ActionResult<IEnumerable<TelemetryData>>> GetTelemetryData(
            [FromQuery] DateTime? since = null) // ?since= (?since=2025-04-19)
        {
            // Если параметр не передан, установка min времемени
            DateTime filterTime = since ?? DateTime.MinValue;

            _logger.LogInformation("[SUCCESS] Получен запрос API на получение телеметрических данных с момента {FilterTime}", filterTime);

            try
            {
                var telemetryData = await _context.TelemetryData
                    .Where(t => t.Timestamp > filterTime) // Фильтрация по времени
                    .OrderBy(t => t.Timestamp) // Сортировка по времени (по возрастанию)
                    .ToListAsync(); // Асинхронное выполнение запроса к базе данных

                _logger.LogInformation("[SUCCESS] Возвращено {Count} записей телеметрии", telemetryData.Count);
                return Ok(telemetryData);
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "[ERROR] Ошибка при получении телеметрических данных с момента {FilterTime}", filterTime);
                return StatusCode(500, "Внутренняя ошибка сервера при получении данных телеметрии");
            }
        }
    }
}