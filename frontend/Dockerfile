FROM node:18 AS builder
WORKDIR /html
COPY ./src ./
RUN npm install -g html-minifier-terser uglify-js clean-css-cli posthtml-cli posthtml-minifier \
    && find . -name '*.html' -exec posthtml --use posthtml-minifier -o {} {} \; \
    && find . -name '*.js' -exec uglifyjs --compress --mangle -o {} -- {} \; \
    && find . -name '*.css' -exec cleancss -o {} -- {} \;

FROM nginx:1.24
COPY wiki-linker.com /etc/nginx/conf.d/default.conf
COPY --from=builder /html /usr/share/nginx/html/
EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
