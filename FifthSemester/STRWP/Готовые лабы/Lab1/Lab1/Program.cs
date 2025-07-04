using System.Web;

internal class Program
{
    private static void Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);
        var app = builder.Build();

        //http://localhost:5039/ZSS?ParmA=19&ParmB=20
        app.MapGet("/ZSS", (string ParmA, string ParmB) =>
        {
            string response = $"GET-Http-ZSS:ParmA = {ParmA}, ParmB = {ParmB}";
            return Results.Text(response, "text/plain");
        });

        app.MapPost("/ZSS", async (HttpContext context) =>
        {
            var form = await context.Request.ReadFormAsync();
            var paramA = form["ParmA"].ToString();
            var paramB = form["ParmB"].ToString();
            string response = $"POST-Http-ZSS:ParmA = {paramA}, ParmB = {paramB}";
            return Results.Text(response, "text/plain");
        });

        app.MapPut("/ZSS", async (HttpContext context) =>
        {
            var form = await context.Request.ReadFormAsync();
            var paramA = form["ParmA"].ToString();
            var paramB = form["ParmB"].ToString();
            string response = $"PUT-Http-ZSS:ParmA = {paramA}, ParmB = {paramB}";
            return Results.Text(response, "text/plain");
        });

        app.MapPost("/sum", async (string X, string Y) =>
        {
            int sum = Convert.ToInt32(X) + Convert.ToInt32(Y);
            return Results.Text($"{sum}", "text/plain");
        });

        //http://localhost:5039/calculate
        app.MapGet("/calculate", async (context) =>
        {
            context.Response.ContentType = "text/html; charset=utf-8";
            await context.Response.SendFileAsync("wwwroot/task5.html");
        });

        app.MapPost("/calculate", async (context) =>
        {
            var requestBody = await new StreamReader(context.Request.Body).ReadToEndAsync();
            var parameters = HttpUtility.ParseQueryString(requestBody);

            int x = int.Parse(parameters["X"]);
            int y = int.Parse(parameters["Y"]);
            int multiplication = x * y;
                
            await context.Response.WriteAsync(multiplication.ToString());
        });

        //http://localhost:5039/calcul
        app.MapGet("/calcul", async (context) =>
        {
            context.Response.ContentType = "text/html; charset=utf-8";
            await context.Response.SendFileAsync("wwwroot/task6.html");
        });

        app.MapPost("/calcul", async (context) =>
        {
            var form = await context.Request.ReadFormAsync();
            var x = int.Parse(form["X"]);
            var y = int.Parse(form["Y"]);
            var multiplication = x * y;

            await context.Response.WriteAsync(multiplication.ToString());
        });

        app.Run();
    }
}