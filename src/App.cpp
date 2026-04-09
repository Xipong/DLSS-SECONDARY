#include "App.hpp"

#include <thread>

namespace dlss_secondary {

App::App(AppConfig config)
    : config_(std::move(config)), logger_(config_.logPath), benchmark_(config_.csvPath) {}

int App::Run() {
    logger_.Info("Starting DLSS secondary-adapter feasibility prototype.");
    logger_.Info("cross_adapter_enabled=" + BoolString(config_.enableCrossAdapter) +
                 " frame_count=" + std::to_string(config_.frameCount));

    AdapterEnumerator enumerator(logger_);
    const EnumeratedAdapters adapters = enumerator.Enumerate();

    Diagnostics diagnostics(logger_);
    diagnostics.PrintAdapterTable(adapters);
    diagnostics.PrintPipelineExpectations();

    if (!adapters.amdPrimary || !adapters.nvidiaSecondary) {
        logger_.Error("Required adapter pair (AMD + NVIDIA RTX) not found. Stopping.");
        return 2;
    }

    Renderer renderer(logger_, *adapters.amdPrimary);
    CrossAdapterBridge bridge(logger_, *adapters.amdPrimary, *adapters.nvidiaSecondary, config_.enableCrossAdapter);

    for (std::uint64_t frame = 0; frame < config_.frameCount; ++frame) {
        const auto frameStart = Clock::now();

        const auto renderStart = Clock::now();
        RenderPacket packet = renderer.RenderFrame(frame);
        std::this_thread::sleep_for(std::chrono::microseconds(1800)); // synthetic GPU render workload
        const auto renderEnd = Clock::now();

        const TransferResult transfer = bridge.TransferForDlss(packet);

        const auto frameEnd = Clock::now();

        FrameMetrics metrics;
        metrics.frameIndex = frame;
        metrics.cpuRenderMs = std::chrono::duration<double, std::milli>(renderEnd - renderStart).count();
        metrics.cpuCrossAdapterMs = transfer.cpuCopyMs;
        metrics.cpuEndToEndMs = std::chrono::duration<double, std::milli>(frameEnd - frameStart).count();
        metrics.gpuRenderMs = 1.8;
        metrics.gpuCopyMs = transfer.gpuCopyMs;
        metrics.estimatedBandwidthGBs = transfer.estimatedBandwidthGBs;
        metrics.crossAdapterEnabled = bridge.Enabled();
        metrics.transferSucceeded = transfer.success;
        metrics.notes = transfer.reason + " | camera=" + packet.cameraMetadata;

        benchmark_.Append(metrics);

        if ((frame % 30) == 0) {
            logger_.Info("frame=" + std::to_string(frame) +
                         " cpu_end_to_end_ms=" + std::to_string(metrics.cpuEndToEndMs) +
                         " transfer_ok=" + BoolString(metrics.transferSucceeded) +
                         " note=" + transfer.reason);
        }
    }

    logger_.Info("Run complete. See bench/metrics.csv and bench/run.log.");
    return 0;
}

} // namespace dlss_secondary
