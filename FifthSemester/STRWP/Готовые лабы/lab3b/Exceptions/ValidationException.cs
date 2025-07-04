using Microsoft.AspNetCore.Mvc.ModelBinding;

namespace lab3b.Exceptions;

public class ValidationException : Exception
{
    public IEnumerable<string> ValidationErrors { get; init; }


    public ValidationException(ModelStateDictionary modelState) : base()
    {
        ValidationErrors = prepareAnswer(modelState);
    }

    private static IEnumerable<string> prepareAnswer(ModelStateDictionary modelState)
    {
        var values = modelState.Values;
        var list = new List<string>();

        foreach (var v in values)
            list.AddRange(v.Errors.Select(e => e.ErrorMessage));

        return list;
    }
}
