#include "Renderer.hpp"
#include "Logger.hpp"

#include <cmath>

namespace dlss_secondary {

Renderer::Renderer(Logger& logger, const AdapterInfo& adapter) : logger_(logger), adapter_(adapter) {
    logger_.Info("Renderer initialized on adapter: " + adapter_.name);
}

RenderPacket Renderer::RenderFrame(std::uint64_t frameIndex) {
    RenderPacket packet;
    packet.frameIndex = frameIndex;
    packet.width = 2560;
    packet.height = 1440;

    constexpr std::size_t colorBytesPerPixel = 8;      // R16G16B16A16_FLOAT
    constexpr std::size_t depthBytesPerPixel = 4;      // D32_FLOAT
    constexpr std::size_t motionBytesPerPixel = 4;     // R16G16_FLOAT

    const std::size_t pixels = static_cast<std::size_t>(packet.width) * packet.height;
    packet.colorBytes = pixels * colorBytesPerPixel;
    packet.depthBytes = pixels * depthBytesPerPixel;
    packet.motionVectorBytes = pixels * motionBytesPerPixel;

    const double t = static_cast<double>(frameIndex) * 0.016;
    std::ostringstream camera;
    camera << "viewOrigin=(" << std::sin(t) * 5.0 << ", 1.5, " << std::cos(t) * 5.0
           << "), jitter=(" << std::sin(t * 13.0) * 0.5 << ", " << std::cos(t * 17.0) * 0.5 << ")";
    packet.cameraMetadata = camera.str();

    return packet;
}

} // namespace dlss_secondary
