using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using System.Linq;

public class HomeController : Controller
{
    private const string AccessKey = "Mode";
    private const string SessionKey = "Mode";

    private readonly AppDbContext _dbContext;

    public HomeController(AppDbContext dbContext)
    {
        _dbContext = dbContext;
    }

    public IActionResult Index(string filterKeyword)
    {
        if (User.IsInRole("Owner"))
        {
            ViewBag.Mode = "owner";
        }
        else
        {
            ViewBag.Mode = "guest";
        }
        var mode = HttpContext.Session.GetString(SessionKey) ?? "guest";
        ViewBag.Mode = mode;

        var links = string.IsNullOrEmpty(filterKeyword)
            ? _dbContext.Links.Include(l => l.Comments).ToList()
            : _dbContext.Links.Include(l => l.Comments).Where(l => l.Keywords.Contains(filterKeyword)).ToList();

        ViewBag.Links = links;
        ViewBag.FilterKeyword = filterKeyword;
        return View();
    }

    [HttpPost]
    public IActionResult SwitchMode(string accessKey)
    {
        if (accessKey == AccessKey)
        {
            HttpContext.Session.SetString(SessionKey, "owner");
        }
        else
        {
            HttpContext.Session.SetString(SessionKey, "guest");
        }

        return RedirectToAction("Index");
    }

    [HttpPost]
    public IActionResult AddLink(string url, string keywords)
    {
        if (HttpContext.Session.GetString(SessionKey) == "owner")
        {
            var link = new LinkModel { Url = url, Keywords = keywords };
            _dbContext.Links.Add(link);
            _dbContext.SaveChanges();
        }

        return RedirectToAction("Index");
    }

    [HttpPost]
    public IActionResult DeleteLink(int id)
    {
        if (HttpContext.Session.GetString(SessionKey) == "owner")
        {
            var link = _dbContext.Links.Find(id);
            if (link != null)
            {
                _dbContext.Links.Remove(link);
                _dbContext.SaveChanges();
            }
        }

        return RedirectToAction("Index");
    }

    [HttpPost]
    public IActionResult EditLink(int id, string url, string keywords)
    {
        if (HttpContext.Session.GetString(SessionKey) == "owner" && !string.IsNullOrEmpty(url) && !string.IsNullOrEmpty(keywords))
        {
            var link = _dbContext.Links.FirstOrDefault(l => l.Id == id);
            if (link != null)
            {
                link.Url = url;
                link.Keywords = keywords;
                _dbContext.SaveChanges();
            }
        }
        return RedirectToAction("Index");
    }

    [HttpPost]
    public IActionResult AddComment(int linkId, string commentText)
    {
        if (string.IsNullOrEmpty(commentText))
        {
            return RedirectToAction("Index");
        }

        var link = _dbContext.Links.FirstOrDefault(l => l.Id == linkId);
        if (link == null)
        {
            return RedirectToAction("Index");
        }

        var newComment = new CommentModel
        {
            Text = commentText,
            CreatedAt = DateTime.Now,
            SessionId = HttpContext.Session.GetString(SessionKey),
            LinkId = linkId 
        };

        _dbContext.Comments.Add(newComment);
        _dbContext.SaveChanges();

        return RedirectToAction("Index");
    }

    [HttpPost]
    public IActionResult EditComment(int commentId, string newText)
    {
        if (!string.IsNullOrEmpty(newText))
        {
            var comment = _dbContext.Comments.FirstOrDefault(c => c.Id == commentId);
            if (comment != null)
            {
                if (HttpContext.Session.GetString(SessionKey) == "owner")
                {
                    comment.Text = newText;
                    _dbContext.SaveChanges();
                }
                else
                {
                    var userSessionId = HttpContext.Session.GetString(SessionKey);
                    if (comment.SessionId == userSessionId)
                    {
                        comment.Text = newText;
                        _dbContext.SaveChanges();
                    }
                }
            }
        }
        return RedirectToAction("Index");
    }

    [HttpPost]
    public IActionResult DeleteComment(int commentId)
    {
        var comment = _dbContext.Comments.FirstOrDefault(c => c.Id == commentId);

        if (comment != null)
        {
            var currentSessionId = HttpContext.Session.GetString(SessionKey);

            if (comment.SessionId == currentSessionId || HttpContext.Session.GetString(SessionKey) == "owner")
            {
                _dbContext.Comments.Remove(comment);
                _dbContext.SaveChanges();
            }
            else
            {
                TempData["ErrorMessage"] = "Вы не можете удалить комментарий другого пользователя.";
            }
        }

        return RedirectToAction("Index");
    }

    [HttpPost]
    public IActionResult Vote(int id, bool isUseful)
    {
        if (HttpContext.Session.GetString(SessionKey) == "owner")
        {
            var link = _dbContext.Links.Find(id);
            if (link != null)
            {
                if (isUseful)
                {
                    link.UsefulCount++;
                }
                else
                {
                    link.UselessCount++;
                }
                _dbContext.SaveChanges();
            }
        }
        return RedirectToAction("Index");
    }
}