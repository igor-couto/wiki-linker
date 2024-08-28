# Stage 1: Build the frontend
FROM node:18 AS build

WORKDIR /app

# Install dependencies
COPY package*.json ./
RUN npm install

# Copy the rest of the application code and build
COPY . .
RUN npm run build

# Stage 2: Serve the frontend with Nginx
FROM nginx:alpine

# Copy the build output to Nginx's web root
COPY --from=build /app/build /usr/share/nginx/html

# Copy a custom nginx config file if needed
COPY nginx.conf /etc/nginx/nginx.conf

# Expose port 80
EXPOSE 80

# Start Nginx server
CMD ["nginx", "-g", "daemon off;"]
