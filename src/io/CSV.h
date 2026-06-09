#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace lcocr {
namespace fs = std::filesystem;

std::vector<std::string> splitCSVLine(const std::string& line);
std::vector<std::vector<std::string>> readCSV(const fs::path& path);

} // namespace lcocr
