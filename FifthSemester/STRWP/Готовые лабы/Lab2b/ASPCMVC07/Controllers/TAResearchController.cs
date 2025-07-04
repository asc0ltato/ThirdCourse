using Microsoft.AspNetCore.Mvc;

namespace ASPCMVC07.Controllers
{
    [Route("it")]
    public class TAResearchController : Controller
    {
        [HttpGet("{n:int}/{string}")]
        public ActionResult M04(int n, string str)
        {
            return Content($"GET:M04:/{n}/{str}");
        }

        [HttpGet]
        [HttpPost]
        [Route("{b:bool}/{letters:alpha}")]
        public ActionResult M05(bool b, string letters)
        {
            string method = Request.Method;
            return Content($"{method}:M05:/{b}/{letters}");
        }

        [HttpGet]
        [HttpDelete]
        [Route("{f:float}/{string:length(2,5)}")]
        public ActionResult M06(float f, string str)
        {
            string method = Request.Method;
            return Content($"{method}:M06:/{f}/{str}");
        }
        [HttpPut]
        [Route("{letters:alpha:length(3,4)}/{n:int:range(100,200)}")]
        public ActionResult M07(string letters, int n)
        {
            return Content($"PUT:M07:/{letters}/{n}");
        }

        [HttpPost]
        [Route("{mail:regex(^\\S+@\\S+\\.\\S+$)}")]
        public ActionResult M08(string mail)
        {
            return Content($"POST:M08:/{mail}");
        }
    }
}