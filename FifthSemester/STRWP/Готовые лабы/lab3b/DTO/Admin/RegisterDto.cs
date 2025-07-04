using System.ComponentModel.DataAnnotations;
namespace lab3b.DTO.Admin;

public class RegisterDto
{
    [Length(5, 32, ErrorMessage = "Логин должен быть от 5 до 32 символов")]
    public string? Username { get; set; }

    [Length(5, 32, ErrorMessage = "Пароль должен быть от 5 до 32 символов")]
    public string? Password { get; set; }
}
