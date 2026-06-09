#pragma once

#include <filesystem>
#include <fstream>
#include <string>

namespace lcocr {
namespace fs = std::filesystem;

class Logger {
public:
    explicit Logger(const fs::path& logFile);
    void info(const std::string& msg);
    void warn(const std::string& msg);
    void error(const std::string& msg);
private:
    std::ofstream file_;
    void write(const std::string& level, const std::string& msg);
};

} // namespace lcocr
