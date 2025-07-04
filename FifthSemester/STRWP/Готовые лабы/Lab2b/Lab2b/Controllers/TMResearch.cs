using Lab2b.Models;
using Microsoft.AspNetCore.Mvc;
using System.Diagnostics;

namespace Lab2b.Controllers
{
    public class TMResearch : Controller
    {
        public ActionResult M01(string id)
        {
            return Content("GET:M01");
        }
        public ActionResult M02()
        {
            return Content("GET:M02");
        }

        public ActionResult M03(string id)
        {
            return Content("GET:M03");
        }

        public ActionResult MXX()
        {
            return Content("GET:MXX");
        }
    }
}
