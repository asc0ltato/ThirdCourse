namespace CentralServer.Models
{
    public class TelemetryData
    {
        public string SourceId { get; set; } = "Unknown";
        public DateTime Timestamp { get; set; }
        public string MeasurementType { get; set; }
        public double Value { get; set; }
        public string Unit { get; set; }
        public string Status { get; set; } = "OK";
        public string ErrorMessage { get; set; }
    }
}
