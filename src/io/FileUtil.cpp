#include "io/FileUtil.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <stdexcept>

namespace lcocr {

std::string trim(const std::string& s) {
    const auto first = s.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const auto last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

std::string quotePath(const fs::path& p) {
    return "\"" + p.u8string() + "\"";
}

std::vector<std::string> readLines(const fs::path& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath.u8string());
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(trim(line));
    }
    return lines;
}

void ensureDirectory(const fs::path& folder) {
    if (!folder.empty()) fs::create_directories(folder);
}

std::string csvEscape(const std::string& value) {
    std::string out = "\"";
    for (char c : value) {
        if (c == '"') out += "\"\"";
        else out += c;
    }
    out += "\"";
    return out;
}

std::string safeStem(const fs::path& p) {
    std::string s = p.stem().u8string();
    for (char& c : s) {
        if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_')) c = '_';
    }
    return s;
}

} // namespace lcocr
