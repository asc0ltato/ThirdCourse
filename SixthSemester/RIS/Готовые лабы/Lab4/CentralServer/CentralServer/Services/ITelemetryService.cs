using CentralServer.Models;

namespace CentralServer.Services
{
    public interface ITelemetryService
    {
        Task<List<TelemetryData>> GetTelemetryDataAsync(string sourceUrl);
        Task<bool> IsSourceAvailableAsync(string sourceUrl);
    }
}
