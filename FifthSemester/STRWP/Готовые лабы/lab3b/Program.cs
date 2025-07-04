using lab3b.Data;
using lab3b.Models;
using Microsoft.AspNetCore.Identity;
using Microsoft.EntityFrameworkCore;

internal class Program
{
    private static void Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);

        builder.Services.AddControllersWithViews();

        var connectionString = builder.Configuration.GetConnectionString("DefaultConnection");

        builder.Services.AddDbContext<AppDbContext>(options =>
                options.UseSqlServer(connectionString));

        builder.Services.AddIdentity<User, IdentityRole>(options =>
        {
            options.Password.RequireDigit = false;
            options.Password.RequireLowercase = false;
            options.Password.RequireUppercase = false;
            options.Password.RequireNonAlphanumeric = false;
            options.Password.RequiredLength = 6;
        })
        .AddEntityFrameworkStores<AppDbContext>()
        .AddDefaultTokenProviders();


        var app = builder.Build();

        using (var scope = app.Services.CreateScope())
        {
            var services = scope.ServiceProvider;
            var roleManager = services.GetRequiredService<RoleManager<IdentityRole>>();
            var userManager = services.GetRequiredService<UserManager<User>>();

            SeedRoles(roleManager).Wait();
            SeedAdminUser(userManager).Wait();
        }

        if (!app.Environment.IsDevelopment())
        {
            app.UseExceptionHandler("/Admin/Error");
            app.UseHsts();
        }

        app.UseHttpsRedirection();
        app.UseStaticFiles();
        app.UseRouting();
        app.UseAuthentication();
        app.UseAuthorization();

        app.MapControllerRoute(
            name: "default",
            pattern: "{controller=Home}/{action=Index}/{id?}");

        app.UseStatusCodePagesWithRedirects("/Admin/Error/{0}");

        app.Run();
    }

    private static async Task SeedRoles(RoleManager<IdentityRole> roleManager)
    {
        if (await roleManager.FindByNameAsync("Administrator") == null)
        {
            await roleManager.CreateAsync(new IdentityRole("Administrator"));
        }

        if (await roleManager.FindByNameAsync("User") == null)
        {
            await roleManager.CreateAsync(new IdentityRole("User"));
        }
    }

    private static async Task SeedAdminUser(UserManager<User> userManager)
    {
        var adminUser = await userManager.FindByNameAsync("admin");
        if (adminUser == null)
        {
            var user = new User
            {
                UserName = "admin",
                NormalizedUserName = "ADMIN"
            };
            var result = await userManager.CreateAsync(user, "admin@111");
            if (result.Succeeded)
            {
                await userManager.AddToRoleAsync(user, "Administrator");
            }
        }
    }
}