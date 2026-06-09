#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace lcocr {
namespace fs = std::filesystem;

std::string trim(const std::string& s);
std::string toLower(std::string s);
std::string quotePath(const fs::path& p);
std::vector<std::string> readLines(const fs::path& filePath);
void ensureDirectory(const fs::path& folder);
std::string csvEscape(const std::string& value);
std::string safeStem(const fs::path& p);

} // namespace lcocr
