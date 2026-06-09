#include "io/Logger.h"
#include "io/FileUtil.h"

#include <chrono>
#include <ctime>
#include <iostream>

namespace lcocr {

Logger::Logger(const fs::path& logFile) {
    ensureDirectory(logFile.parent_path());
    file_.open(logFile, std::ios::app);
}

void Logger::info(const std::string& msg) { write("INFO", msg); }
void Logger::warn(const std::string& msg) { write("WARN", msg); }
void Logger::error(const std::string& msg) { write("ERROR", msg); }

void Logger::write(const std::string& level, const std::string& msg) {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string line = "[" + level + "] " + std::string(std::ctime(&now));
    if (!line.empty() && line.back() == '\n') line.pop_back();
    line += " - " + msg;
    std::cout << line << "\n";
    if (file_.is_open()) file_ << line << "\n";
}

} // namespace lcocr
