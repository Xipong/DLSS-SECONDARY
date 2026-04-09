#include "Diagnostics.hpp"
#include "Logger.hpp"

namespace dlss_secondary {

Diagnostics::Diagnostics(Logger& logger) : logger_(logger) {}

void Diagnostics::PrintAdapterTable(const EnumeratedAdapters& adapters) {
    logger_.Info("Detected adapters: " + std::to_string(adapters.all.size()));
    for (const auto& adapter : adapters.all) {
        std::ostringstream line;
        line << "name='" << adapter.name << "' vendor=0x" << std::hex << adapter.vendorId << std::dec
             << " device=0x" << std::hex << adapter.deviceId << std::dec
             << " luid=(" << adapter.luidHighPart << ":" << adapter.luidLowPart << ")"
             << " dedicatedVRAMMB=" << (adapter.dedicatedVideoMemory / (1024 * 1024))
             << " sharedSysMemMB=" << (adapter.sharedSystemMemory / (1024 * 1024))
             << " d3d12=" << BoolString(adapter.supportsD3D12)
             << " crossAdapterHeap=" << BoolString(adapter.supportsCrossAdapterHeap)
             << " software=" << BoolString(adapter.isSoftware);
        logger_.Info(line.str());
    }
}

void Diagnostics::PrintPipelineExpectations() {
    logger_.Info("DLSS-SR required inputs: HDR color, depth, motion vectors, exposure, camera jitter, frame index.");
    logger_.Info("Prototype focus: measure inter-adapter transfer + sync overhead before attempting Streamline integration.");
    logger_.Info("DLSS Frame Generation is treated as out-of-scope until optical-flow and latency constraints are proven.");
}

} // namespace dlss_secondary
