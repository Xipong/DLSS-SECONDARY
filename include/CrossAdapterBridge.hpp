#pragma once

#include "AdapterEnumerator.hpp"
#include "Common.hpp"
#include "Renderer.hpp"

namespace dlss_secondary {

struct TransferResult {
    bool success = false;
    double cpuCopyMs = 0.0;
    double gpuCopyMs = 0.0;
    double estimatedBandwidthGBs = 0.0;
    std::string reason;
};

class Logger;

class CrossAdapterBridge {
public:
    CrossAdapterBridge(Logger& logger, const AdapterInfo& source, const AdapterInfo& destination, bool enabled);
    TransferResult TransferForDlss(const RenderPacket& packet);
    bool Enabled() const;

private:
    Logger& logger_;
    AdapterInfo source_;
    AdapterInfo destination_;
    bool enabled_ = false;
};

} // namespace dlss_secondary
