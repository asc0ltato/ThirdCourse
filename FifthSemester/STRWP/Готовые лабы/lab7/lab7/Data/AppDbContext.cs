using Microsoft.EntityFrameworkCore;

public class AppDbContext : DbContext
{
    public DbSet<LinkModel> Links { get; set; }
    public DbSet<CommentModel> Comments { get; set; }

    public AppDbContext(DbContextOptions<AppDbContext> options) : base(options)
    {
        Database.EnsureCreated();
    }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        modelBuilder.Entity<CommentModel>()
            .HasOne(c => c.Link)
            .WithMany(l => l.Comments)
            .HasForeignKey(c => c.LinkId)
            .OnDelete(DeleteBehavior.Cascade);
    }
}
