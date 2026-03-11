FROM ubuntu:24.04

# Install build tools
RUN apt-get update && apt-get install -y \
    g++ \
    curl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy source files
COPY crow_app.cpp .
COPY index.html .
COPY crow_all.h .

# Download Crow header + compile
RUN curl -fsSL https://sourceforge.net/projects/crow-framework.mirror/files/v1.2.0/crow_all.h/download -o crow_all.h \
    && g++ -std=c++17 -O3 -pthread crow_app.cpp -o crow_app \
    && chmod +x crow_app

EXPOSE 8080
HEALTHCHECK --interval=30s --timeout=3s --start-period=5s --retries=3 \
  CMD curl -f http://localhost:8080/ || exit 1

CMD ["./crow_app"]
