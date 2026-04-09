#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace dlss_secondary {

using Clock = std::chrono::steady_clock;
using Microseconds = std::chrono::microseconds;

struct GpuTimerSample {
    std::string label;
    double milliseconds = 0.0;
};

struct CpuTimerSample {
    std::string label;
    double milliseconds = 0.0;
};

inline std::string BoolString(bool value) {
    return value ? "true" : "false";
}

} // namespace dlss_secondary
