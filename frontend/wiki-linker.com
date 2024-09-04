server {
    listen 80;

    root /usr/share/nginx/html/wiki-linker.com;
    index index.html;

    location / {
        try_files $uri $uri/ $uri.html =404;
        add_header Cache-Control "public, max-age=604800";
        expires 7d;
    }

    location /api/ {
        proxy_pass http://localhost:50030/api/;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
    }
}