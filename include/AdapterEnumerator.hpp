#pragma once

#include "Common.hpp"

namespace dlss_secondary {

struct AdapterInfo {
    std::string name;
    std::uint64_t dedicatedVideoMemory = 0;
    std::uint64_t sharedSystemMemory = 0;
    std::uint32_t vendorId = 0;
    std::uint32_t deviceId = 0;
    std::uint32_t luidLowPart = 0;
    std::int32_t luidHighPart = 0;
    bool supportsD3D12 = false;
    bool supportsCrossAdapterHeap = false;
    bool isSoftware = false;
};

struct EnumeratedAdapters {
    std::vector<AdapterInfo> all;
    std::optional<AdapterInfo> amdPrimary;
    std::optional<AdapterInfo> nvidiaSecondary;
};

class Logger;

class AdapterEnumerator {
public:
    explicit AdapterEnumerator(Logger& logger);
    EnumeratedAdapters Enumerate();

private:
    Logger& logger_;
};

} // namespace dlss_secondary
