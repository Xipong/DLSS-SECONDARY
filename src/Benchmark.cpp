#include "Benchmark.hpp"

namespace dlss_secondary {

BenchmarkWriter::BenchmarkWriter(const std::filesystem::path& csvPath) : csvPath_(csvPath) {
    std::filesystem::create_directories(csvPath_.parent_path());
}

void BenchmarkWriter::WriteHeaderIfNeeded() {
    if (headerWritten_ && std::filesystem::exists(csvPath_)) {
        return;
    }

    std::ofstream out(csvPath_, std::ios::out | std::ios::trunc);
    out << "frame_index,cpu_render_ms,cpu_cross_adapter_ms,cpu_end_to_end_ms,gpu_render_ms,gpu_copy_ms,estimated_bandwidth_gbs,cross_adapter_enabled,transfer_succeeded,notes\n";
    headerWritten_ = true;
}

void BenchmarkWriter::Append(const FrameMetrics& metrics) {
    WriteHeaderIfNeeded();

    std::ofstream out(csvPath_, std::ios::out | std::ios::app);
    out << metrics.frameIndex << ','
        << std::fixed << std::setprecision(4)
        << metrics.cpuRenderMs << ','
        << metrics.cpuCrossAdapterMs << ','
        << metrics.cpuEndToEndMs << ','
        << metrics.gpuRenderMs << ','
        << metrics.gpuCopyMs << ','
        << metrics.estimatedBandwidthGBs << ','
        << (metrics.crossAdapterEnabled ? 1 : 0) << ','
        << (metrics.transferSucceeded ? 1 : 0) << ','
        << '"' << metrics.notes << '"'
        << '\n';
}

} // namespace dlss_secondary
