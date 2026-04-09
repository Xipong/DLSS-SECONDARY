#include "Logger.hpp"

#include <ctime>

namespace dlss_secondary {

namespace {
std::string Timestamp() {
    const auto now = std::time(nullptr);
    std::tm tm {};
#ifdef _WIN32
    localtime_s(&tm, &now);
#else
    localtime_r(&now, &tm);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
} // namespace

Logger::Logger(const std::filesystem::path& logPath) {
    std::filesystem::create_directories(logPath.parent_path());
    stream_.open(logPath, std::ios::out | std::ios::trunc);
}

void Logger::Info(const std::string& message) {
    Write("INFO", message);
}

void Logger::Warn(const std::string& message) {
    Write("WARN", message);
}

void Logger::Error(const std::string& message) {
    Write("ERROR", message);
}

void Logger::Write(const char* level, const std::string& message) {
    const auto line = Timestamp() + " [" + level + "] " + message;
    std::cout << line << '\n';
    if (stream_.is_open()) {
        stream_ << line << '\n';
        stream_.flush();
    }
}

} // namespace dlss_secondary
