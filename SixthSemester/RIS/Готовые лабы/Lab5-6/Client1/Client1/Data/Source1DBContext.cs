using Client1.Models;
using Microsoft.EntityFrameworkCore;

namespace Client1.Data
{
    public class Source1DBContext : DbContext
    {
        public Source1DBContext(DbContextOptions<Source1DBContext> options) : base(options) { }

        public DbSet<TelemetryData> TelemetryData { get; set; } = null!;
        public DbSet<ReceivedTelemetryData> ReceivedTelemetry { get; set; } = null!;
    }
}