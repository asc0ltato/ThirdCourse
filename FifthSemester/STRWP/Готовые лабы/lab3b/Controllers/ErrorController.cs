using Microsoft.AspNetCore.Mvc;

namespace lab3b.Controllers
{
    public class ErrorController : Controller
    {
        [Route("/Admin/Error/{statusCode}")]
        public IActionResult HttpStatusCodeHandler(int statusCode)
        {
            switch (statusCode)
            {
                case 404:
                    ViewBag.ErrorMessage = "К сожалению, запрошенный вами ресурс не найден";
                    break;
            }
            return View("Error");
        }
    }
}
