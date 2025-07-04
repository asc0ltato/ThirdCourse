using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
namespace lab3b.Controllers;

[Authorize(Roles = "Employee, Master")]
public class CalcController : Controller
{
    public IActionResult Index(string? press, float? x, float? y)
    {
        ViewBag.press = press;
        ViewBag.x = x;
        ViewBag.y = y;
        ViewBag.z = 0;
        return View("Calc");
    }

    [HttpPost]
    public IActionResult Sum([FromForm] float? x, [FromForm] float? y)
    {
        Calculate("+", x, y);
        return View("Calc");
    }

    [HttpPost]
    public IActionResult Sub(float? x, float? y)
    {
        Calculate("-", x, y);
        return View("Calc");
    }

    [HttpPost]
    public IActionResult Mul(float? x, float? y)
    {
        Calculate("*", x, y);
        return View("Calc");
    }

    [HttpPost]
    public IActionResult Div(float? x, float? y)
    {
        Calculate("/", x, y);
        return View("Calc");
    }

    private void Calculate(string press, float? x, float? y)
    {
        float? z = null;

        if (x == null && y == null)
        {
            x = 0;
            y = 0;
            ViewBag.Error = "-- ERROR --";
        }
        else if (y == null)
        {
            y = 0;
            ViewBag.Error = "-- ERROR --";
        }
        else if (x == null)
        {
            x = 0;
            ViewBag.Error = "-- ERROR --";
        }

        switch (press)
        {
            case "+":
                z = x + y;
                break;
            case "-":
                z = x - y;
                break;
            case "*":
                z = x * y;
                break;
            case "/":
                if (y == 0) ViewBag.Error = "-- ERROR --";
                else z = x / y;
                break;
        }

        ViewBag.press = press;
        ViewBag.x = x;
        ViewBag.y = y;
        ViewBag.z = z ?? 0;
    }
}
