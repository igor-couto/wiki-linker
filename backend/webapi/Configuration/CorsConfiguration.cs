namespace WikiLinker.Configuration;

public static class CorsConfiguration
{
    public static void AddCorsConfiguration(this IServiceCollection services)
    {
        services.AddCors(options =>
        {
            options.AddPolicy("AllowAllOrigins",
                builder => builder
                    .AllowAnyOrigin()
                    .AllowAnyHeader()
                    .AllowAnyMethod()
                    .WithExposedHeaders("Location")
            );
        });
    }

    public static void UseCorsConfiguration(this WebApplication app)
    {
        app.UseCors("AllowAllOrigins");
    }
}