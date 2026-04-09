#include "CrossAdapterBridge.hpp"
#include "Logger.hpp"

#include <thread>

namespace dlss_secondary {

CrossAdapterBridge::CrossAdapterBridge(
    Logger& logger,
    const AdapterInfo& source,
    const AdapterInfo& destination,
    const bool enabled)
    : logger_(logger), source_(source), destination_(destination), enabled_(enabled) {
    logger_.Info("CrossAdapterBridge source=" + source_.name + " destination=" + destination_.name +
                 " enabled=" + BoolString(enabled_));
}

bool CrossAdapterBridge::Enabled() const {
    return enabled_;
}

TransferResult CrossAdapterBridge::TransferForDlss(const RenderPacket& packet) {
    TransferResult result;

    if (!enabled_) {
        result.success = false;
        result.reason = "Cross-adapter path disabled by configuration.";
        return result;
    }

    if (!source_.supportsCrossAdapterHeap || !destination_.supportsCrossAdapterHeap) {
        result.success = false;
        result.reason = "One or both adapters report no cross-adapter heap support.";
        return result;
    }

    const std::size_t totalBytes = packet.colorBytes + packet.depthBytes + packet.motionVectorBytes;
    constexpr double effectiveInterconnectGBs = 14.0; // optimistic PCIe + overhead estimate.
    const double estimatedMs = (static_cast<double>(totalBytes) / (1024.0 * 1024.0 * 1024.0)) / effectiveInterconnectGBs * 1000.0;

    // Simulate copy cost in prototype path. Real path will use copy queue + fences.
    const auto start = Clock::now();
    std::this_thread::sleep_for(Microseconds(static_cast<int>(estimatedMs * 1000.0) + 200));
    const auto end = Clock::now();

    result.success = true;
    result.cpuCopyMs = std::chrono::duration<double, std::milli>(end - start).count();
    result.gpuCopyMs = estimatedMs;
    result.estimatedBandwidthGBs = static_cast<double>(totalBytes) / (1024.0 * 1024.0 * 1024.0) / (result.cpuCopyMs / 1000.0);

    if (result.cpuCopyMs > 5.0) {
        result.reason = "Copy cost likely too high for stable frame pacing at high refresh rates.";
    } else {
        result.reason = "Transfer completed within provisional budget.";
    }

    return result;
}

} // namespace dlss_secondary
