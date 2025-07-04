using System.ComponentModel.DataAnnotations;
namespace lab3b.DTO.Admin;

public class ChangePasswordDto
{
    public string? Password { get; set; }

    [Length(5, 32, ErrorMessage = "Пароль должен быть от 5 до 32 символов")]
    public string? NewPassword { get; set; }
}
