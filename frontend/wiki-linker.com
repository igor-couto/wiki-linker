server {
    listen 80;
    listen [::]:80;
    server_name localhost;
    charset utf-8;
    
    root /usr/share/nginx/html;
    index index.html;

    error_page 404 404.html;
    error_page 500 502 503 504 /50x.html;

    location = 404.html {
        internal;
    }

    location = 50x.html {
        internal;
    }

    location / {
        try_files $uri $uri/ $uri.html =404;
        add_header Cache-Control "public, max-age=604800";
        expires 7d;
    }
}