using ASPCMVC04.Models;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using System.Diagnostics;

namespace ASPCMVC04.Controllers
{
    public class StatusController : Controller
    {
        private static readonly Random _random = new Random();

        [HttpGet]
        public IActionResult S200()
        {
            /*return new EmptyResult();*/
            int statusCode = _random.Next(200, 300);
            return StatusCode(statusCode, $"Успешный ответ: {statusCode}.");
        }

        [HttpGet]
        public IActionResult S300()
        {
            /*return Redirect("https://example.com");*/
            int statusCode = _random.Next(300, 400);
            return StatusCode(statusCode, $"Перенаправление: {statusCode}.");
        }

        [HttpGet]
        public IActionResult S500()
        {
            //int zero = 0;
            //int result = 1 / zero;
            int statusCode = _random.Next(500, 600);
            return StatusCode(statusCode, $"Ошибка сервера: {statusCode}.");
        }
    }
}