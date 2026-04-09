#include "App.hpp"

#include <cstdlib>
#include <string>

int main(int argc, char** argv) {
    dlss_secondary::AppConfig config;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--no-cross-adapter") {
            config.enableCrossAdapter = false;
        } else if (arg == "--frames" && i + 1 < argc) {
            config.frameCount = static_cast<std::uint32_t>(std::strtoul(argv[++i], nullptr, 10));
        }
    }

    dlss_secondary::App app(config);
    return app.Run();
}
