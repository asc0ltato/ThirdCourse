using Microsoft.EntityFrameworkCore;
using Client2.Services;
using Client2.Data;

var builder = WebApplication.CreateBuilder(args);
builder.Services.AddDbContext<Source2DBContext>(options =>
    options.UseSqlServer(builder.Configuration.GetConnectionString("Source2DB")));

// Add services to the container.

builder.Services.AddControllers();
// Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

builder.Services.AddHttpClient(); // HttpClientFactory
builder.Services.AddHostedService<SourceDataGeneratorService>();
builder.Services.AddLogging(); // Логи

var app = builder.Build();

// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseAuthorization();

app.MapControllers();

app.Run();
