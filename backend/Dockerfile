FROM mcr.microsoft.com/dotnet/sdk:8.0-jammy-arm64v8 AS build
WORKDIR /source
COPY . .
RUN dotnet restore "./webapi/WikiLinker.csproj" --disable-parallel --runtime linux-arm64
RUN dotnet publish "./webapi/WikiLinker.csproj" -c Release -o /app --no-restore --runtime linux-arm64 --self-contained true

FROM mcr.microsoft.com/dotnet/aspnet:8.0-jammy-arm64v8 AS run
ENV DOTNET_SYSTEM_GLOBALIZATION_INVARIANT=false
ENV ASPNETCORE_ENVIRONMENT=Production
ENV ASPNETCORE_URLS="http://+:50030"
ENV ASPNETCORE_HTTP_PORT=50030

# Install dependencies for C++ compilation
RUN apt-get update && apt-get upgrade -y && apt-get install -y g++

RUN adduser --disabled-password --home /app dotnetuser && chown -R dotnetuser /app

WORKDIR /app

# Copy the .NET build output
COPY --from=build /app .

# Copy and build the C++ application
COPY ./anatex /app/anatex
WORKDIR /app/anatex

RUN g++ src/main.cpp src/Anatex.cpp src/Trie.cpp -o anatex -w

COPY entrypoint.sh /app/entrypoint.sh
RUN chmod +x /app/entrypoint.sh

USER dotnetuser

EXPOSE 50030

ENTRYPOINT ["/app/entrypoint.sh"]