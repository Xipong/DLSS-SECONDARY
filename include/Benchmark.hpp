#pragma once

#include "Common.hpp"

namespace dlss_secondary {

struct FrameMetrics {
    std::uint64_t frameIndex = 0;
    double cpuRenderMs = 0.0;
    double cpuCrossAdapterMs = 0.0;
    double cpuEndToEndMs = 0.0;
    double gpuRenderMs = 0.0;
    double gpuCopyMs = 0.0;
    double estimatedBandwidthGBs = 0.0;
    bool crossAdapterEnabled = false;
    bool transferSucceeded = false;
    std::string notes;
};

class BenchmarkWriter {
public:
    explicit BenchmarkWriter(const std::filesystem::path& csvPath);
    void WriteHeaderIfNeeded();
    void Append(const FrameMetrics& metrics);

private:
    std::filesystem::path csvPath_;
    bool headerWritten_ = false;
};

} // namespace dlss_secondary
