#include "crow_all.h"
#include <chrono>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <fstream>

// Your benchmark function
double benchmark_loop(long long N) {
    volatile double x = 0.0;
    auto start = std::chrono::high_resolution_clock::now();
    
    for (long long i = 0; i < N; ++i) {
        x += 0.0000001;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

// Serve static files (index.html)
std::string read_file(const std::string& path) {
    std::ifstream file(path);
    return std::string((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
}

int main() {
    crow::SimpleApp app;
    
    // Serve index.html
    CROW_ROUTE(app, "/")([](){
        return read_file("index.html");
    });
    
    // API endpoint
    CROW_ROUTE(app, "/api/bench")([&](const crow::request& req){
        const long long N = 10'000'000;
        const int runs = 3;
        std::vector<double> times;
        times.reserve(runs);
        
        for (int i = 0; i < runs; ++i) {
            times.push_back(benchmark_loop(N));
        }
        
        double avg = 0.0;
        for (double t : times) avg += t;
        avg /= runs;
        
        crow::json::wvalue result;
        result["cpp"]["language"] = "cpp";
        result["cpp"]["iterations"] = static_cast<int64_t>(N);
        result["cpp"]["runs"] = runs;
        result["cpp"]["avg_time"] = avg;
        result["python"]["language"] = "python";
        result["python"]["iterations"] = static_cast<int64_t>(N);
        result["python"]["runs"] = runs;
        result["python"]["avg_time"] = avg * 10.0;  // Simulate Python
        
        return crow::response(result);
    });
    
    // Serve static files (CSS/JS in HTML works via inline)
    CROW_ROUTE(app, "/<path>")([](std::string path){
        return read_file(path);
    });
    
    app.port(8080)
       .multithreaded()
       .run();
}
