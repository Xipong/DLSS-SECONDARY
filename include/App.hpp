#pragma once

#include "AdapterEnumerator.hpp"
#include "Benchmark.hpp"
#include "CrossAdapterBridge.hpp"
#include "Diagnostics.hpp"
#include "Logger.hpp"
#include "Renderer.hpp"

namespace dlss_secondary {

struct AppConfig {
    std::uint32_t frameCount = 120;
    bool enableCrossAdapter = true;
    std::filesystem::path logPath = "bench/run.log";
    std::filesystem::path csvPath = "bench/metrics.csv";
};

class App {
public:
    explicit App(AppConfig config);
    int Run();

private:
    AppConfig config_;
    Logger logger_;
    BenchmarkWriter benchmark_;
};

} // namespace dlss_secondary
