#pragma once

#include "Common.hpp"

namespace dlss_secondary {

class Logger {
public:
    explicit Logger(const std::filesystem::path& logPath);

    void Info(const std::string& message);
    void Warn(const std::string& message);
    void Error(const std::string& message);

private:
    void Write(const char* level, const std::string& message);

    std::ofstream stream_;
};

} // namespace dlss_secondary
