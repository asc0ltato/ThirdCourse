using Microsoft.EntityFrameworkCore;
using CentralServer.Models;

namespace CentralServer.Data
{
    public class CentralDBContext : DbContext
    {
        public CentralDBContext(DbContextOptions<CentralDBContext> options) : base(options) { }

        public DbSet<TelemetryData> TelemetryData { get; set; } = null!;

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);
            modelBuilder.Entity<TelemetryData>()
                .HasIndex(t => new { t.SourceIdentifier, t.Timestamp });
        }
    }
}
