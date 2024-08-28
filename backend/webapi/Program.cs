using System.Security.Cryptography;
using System.Text;
using Microsoft.AspNetCore.Mvc;
using WikiLinker;
using WikiLinker.Configuration;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();
builder.Services.AddCorsConfiguration();

var app = builder.Build();

app.UseSwagger();
app.UseSwaggerUI();
app.UseCorsConfiguration();

// Access the directory paths directly from the configuration
var configuration = app.Configuration;
var inputDirectory = Path.Combine(AppContext.BaseDirectory, configuration["Directories:InputDirectory"]);
var outputDirectory = Path.Combine(AppContext.BaseDirectory, configuration["Directories:OutputDirectory"]);

// Ensure the directories exist
if (!Directory.Exists(inputDirectory))
    Directory.CreateDirectory(inputDirectory);

if (!Directory.Exists(outputDirectory))
    Directory.CreateDirectory(outputDirectory);

app.MapPost("/api/text", async ([FromBody] AnnotateTextRequest annotateTextRequest) =>
{
    var id = ComputeSha256Hash(annotateTextRequest.Text);
    var inputFilePath = Path.Combine(inputDirectory, $"{id}.txt");

    if (File.Exists(inputFilePath))
        return Results.Accepted(id);

    await File.WriteAllTextAsync(inputFilePath, annotateTextRequest.Text);
    return Results.Accepted(id);
});

app.MapGet("/api/text/{id}", async (string id) =>
{
    var outputFilePath = Path.Combine(outputDirectory, $"{id}.html");

    if(!File.Exists(outputFilePath))
        return Results.NotFound();

    var annotatedText = await File.ReadAllTextAsync(outputFilePath);
    File.Delete(outputFilePath);

    var inputFilePath = Path.Combine(inputDirectory, $"{id}.txt");
    File.Delete(inputFilePath);

    return Results.Ok(new { annotatedText });
});

static string ComputeSha256Hash(string rawData)
{
    var bytes = SHA256.HashData(Encoding.UTF8.GetBytes(rawData));

    var builder = new StringBuilder();
    foreach (var b in bytes)
        builder.Append(b.ToString("x2"));

    return builder.ToString();
}

app.Run();