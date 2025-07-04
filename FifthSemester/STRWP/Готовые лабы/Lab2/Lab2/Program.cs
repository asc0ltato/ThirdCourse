using System.Net.WebSockets;
using System.Text;

internal class Program
{
    private static void Main(string[] args)
    {
        var builder = WebApplication.CreateBuilder(args);
        var app = builder.Build();

        app.UseWebSockets();

        app.MapGet("/", async (context) =>
        {
            context.Response.ContentType = "text/html; charset=utf-8";
            await context.Response.SendFileAsync("wwwroot/index.html");
        });

        app.Map("/ws", async (context) =>
        {
            if (context.WebSockets.IsWebSocketRequest)
            {
                var webSocket = await context.WebSockets.AcceptWebSocketAsync();

                try
                {
                    while (webSocket.State == WebSocketState.Open)
                    {
                        var time = Encoding.UTF8.GetBytes(DateTime.Now.ToString("hh:mm:ss"));
                        await webSocket.SendAsync(new ArraySegment<byte>(time, 0, time.Length), WebSocketMessageType.Text, true, CancellationToken.None);
                        await Task.Delay(2000);
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"WebSocket error: {ex.Message}");
                }
            }
            else
            {
                context.Response.StatusCode = 400;
            }
        });

        app.Run();
    }
}