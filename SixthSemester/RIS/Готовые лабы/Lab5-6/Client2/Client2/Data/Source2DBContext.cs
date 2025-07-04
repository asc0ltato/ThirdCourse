using Client2.Models;
using Microsoft.EntityFrameworkCore;

namespace Client2.Data
{
    public class Source2DBContext : DbContext
    {
        public Source2DBContext(DbContextOptions<Source2DBContext> options) : base(options) { }

        public DbSet<TelemetryData> TelemetryData { get; set; } = null!;
        public DbSet<ReceivedTelemetryData> ReceivedTelemetry { get; set; } = null!;
    }
}