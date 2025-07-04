using Client2.Models;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;
using Client2.Data;     

namespace Client2.Controllers
{
    [Route("api/telemetry")]
    [ApiController]
    public class TelemetryPushController : ControllerBase
    {
        private readonly Source2DBContext _context;
        private readonly ILogger<TelemetryPushController> _logger;

        public TelemetryPushController(Source2DBContext context, ILogger<TelemetryPushController> logger)
        {
            _context = context;
            _logger = logger;
        }

        // Прием данных POST /api/telemetry/push
        [HttpPost("push")]
        public async Task<IActionResult> ReceivePushData([FromBody] List<TelemetryData> receivedData)
        {
            // Если тело пустое или null - ошибка
            if (receivedData == null || !receivedData.Any())
            {
                _logger.LogWarning("[WARNING] Получен пустой или нулевой пакет данных с помощью push-запроса");
                return BadRequest("Пакет данных не может быть нулевым или пустым");
            }

            _logger.LogInformation("[SUCCESS] Получены записи телеметрии {Count} с помощью push-запроса", receivedData.Count);

            try
            {
                // Преобразование входных данных TelemetryData -> ReceivedTelemetryData
                // Добавляется метка времени получения
                var receivedEntities = receivedData.Select(d => new ReceivedTelemetryData
                {
                    SourceIdentifier = d.SourceIdentifier,
                    ObjectId = d.ObjectId,
                    Timestamp = d.Timestamp,
                    Value = d.Value,
                    ReceivedTimestampUtc = DateTime.UtcNow // Время получения сервером
                }).ToList();

                // Асинхронное добавление всех данных в таблицу ReceivedTelemetry
                await _context.ReceivedTelemetry.AddRangeAsync(receivedEntities);

                int savedCount = await _context.SaveChangesAsync();

                _logger.LogInformation("[SUCCESS] Успешно сохранены {Count} полученные записи в таблице полученной телеметрии", receivedEntities.Count);
                return Ok($"Успешно обработанные и сохраненные {receivedEntities.Count} записей");
            }
            catch (DbUpdateException ex)
            {
                _logger.LogError(ex, "[ERROR] Ошибка базы данных при сохранении полученных телеметрических данных");
                return StatusCode(500, $"При сохранении данных произошла ошибка базы данных: {ex.InnerException?.Message ?? ex.Message}");
            }
            catch (Exception ex)
            {
                _logger.LogError(ex, "[ERROR] Ошибка при обработке полученных телеметрических данных");
                return StatusCode(500, $"Произошла непредвиденная ошибка: {ex.Message}");
            }
        }
    }
}