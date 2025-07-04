internal class Program
{
    private static void Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);
        builder.Services.AddControllersWithViews();
        var app = builder.Build();

        app.UseRouting();

        app.UseEndpoints(endpoints =>
        {
            endpoints.MapControllerRoute(
                name: "M01",
                pattern: "{controller}/{action}/{id?}",
                defaults: new { controller = "TMResearch", action = "M01" });

            endpoints.MapControllerRoute(
                name: "M02",
                pattern: "V2/{controller}/{action}",
                defaults: new { controller = "TMResearch", action = "M02" });

            endpoints.MapControllerRoute(
                name: "M03",
                pattern: "V3/{controller}/{id?}/{action}",
                defaults: new { controller = "TMResearch", action = "M03" });

            endpoints.MapControllerRoute(
                name: "MXX",
                pattern: "{*uri}",
                defaults: new { controller = "TMResearch", action = "MXX" });

        }); 

        app.Run();
    }
}