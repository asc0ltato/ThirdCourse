namespace lab3b.Models;

public class RUser : User
{
    public IEnumerable<string> Roles { get; set; }

    public RUser(User user, IEnumerable<string> roles) : base(user)
    {
        Roles = roles;
    }
}
