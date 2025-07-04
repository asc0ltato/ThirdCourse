using Microsoft.AspNetCore.Mvc;

namespace CentralServer.Controllers
{
    public class CentralTelemetryController : Controller
    {
        public IActionResult Index()
        {
            return View();
        }
    }
}