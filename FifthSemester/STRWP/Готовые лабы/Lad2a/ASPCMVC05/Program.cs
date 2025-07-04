internal class Program
{
    private static void Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);
        builder.Services.AddControllersWithViews();
        var app = builder.Build();

        app.MapControllerRoute(
            name: "default",
            pattern: "{controller=Parm}/{action=Index}/{id?}");

        app.Run();
    }
}