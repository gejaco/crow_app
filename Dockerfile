FROM ubuntu:24.04
RUN apt-get update && apt-get install -y g++ curl
WORKDIR /app
COPY . .
RUN curl -L https://raw.githubusercontent.com/CrowCpp/Crow/master/include/crow_all.h -o crow_all.h \
    && g++ -std=c++17 -O3 -pthread crow_app.cpp -o crow_app
EXPOSE 8080
CMD ["./crow_app"]
