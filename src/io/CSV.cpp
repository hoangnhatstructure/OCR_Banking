#include "io/CSV.h"
#include "io/FileUtil.h"

namespace lcocr {

std::vector<std::string> splitCSVLine(const std::string& line) {
    std::vector<std::string> result;
    std::string cell;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"') {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                cell.push_back('"');
                ++i;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            result.push_back(trim(cell));
            cell.clear();
        } else {
            cell.push_back(c);
        }
    }
    result.push_back(trim(cell));
    return result;
}

std::vector<std::vector<std::string>> readCSV(const fs::path& path) {
    auto lines = readLines(path);
    std::vector<std::vector<std::string>> rows;
    for (const auto& line : lines) {
        if (!line.empty()) rows.push_back(splitCSVLine(line));
    }
    return rows;
}

} // namespace lcocr
