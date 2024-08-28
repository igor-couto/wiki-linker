using System.ComponentModel.DataAnnotations;

namespace WikiLinker;

public record AnnotateTextRequest
{
    [StringLength(10000)]
    public required string Text { get; set; }
}