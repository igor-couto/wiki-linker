FROM node:latest AS builder
WORKDIR /html
COPY ./src ./
RUN npm install -g html-minifier-terser uglify-js clean-css-cli posthtml-cli posthtml-minifier \
    && find . -name '*.html' -exec posthtml --use posthtml-minifier -o {} {} \; \
    && find . -name '*.js' -exec uglifyjs --compress --mangle -o {} -- {} \; \
    && find . -name '*.css' -exec cleancss -o {} -- {} \;

FROM nginx:latest
COPY wiki-linker.com /etc/nginx/sites-available/
RUN ln -sf /etc/nginx/sites-available/wiki-linker.com /etc/nginx/sites-enabled/
COPY --from=builder /html /usr/share/nginx/html/wiki-linker.com
EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
