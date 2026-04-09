#pragma once

#include "AdapterEnumerator.hpp"
#include "Common.hpp"

namespace dlss_secondary {

struct RenderPacket {
    std::uint32_t width = 1920;
    std::uint32_t height = 1080;
    std::size_t colorBytes = 0;
    std::size_t depthBytes = 0;
    std::size_t motionVectorBytes = 0;
    std::uint64_t frameIndex = 0;
    std::string cameraMetadata;
};

class Logger;

class Renderer {
public:
    Renderer(Logger& logger, const AdapterInfo& adapter);
    RenderPacket RenderFrame(std::uint64_t frameIndex);

private:
    Logger& logger_;
    AdapterInfo adapter_;
};

} // namespace dlss_secondary
