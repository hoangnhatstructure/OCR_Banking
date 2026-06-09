#include "core/LineUnwrapper.h"
#include "io/FileUtil.h"

namespace lcocr {

bool LineUnwrapper::startsWithAlias(const std::string& line, const std::vector<AliasDef>& aliases) const {
    std::string lowerLine = toLower(line);
    for (const auto& alias : aliases) {
        std::string lowerAlias = toLower(alias.alias);
        if (lowerLine.rfind(lowerAlias, 0) == 0) return true;
    }
    return false;
}

std::vector<std::string> LineUnwrapper::unwrap(const std::vector<OCRPage>& pages, const std::vector<AliasDef>& aliases) const {
    std::vector<std::string> result;
    std::string buffer;

    for (const auto& page : pages) {
        for (const auto& rawLine : page.lines) {
            std::string line = trim(rawLine);
            if (line.empty()) continue;

            bool isNewField = startsWithAlias(line, aliases);
            if (isNewField) {
                if (!buffer.empty()) result.push_back(buffer);
                buffer = line;
            } else {
                if (!buffer.empty()) buffer += " " + line;
                else buffer = line;
            }
        }
    }

    if (!buffer.empty()) result.push_back(buffer);
    return result;
}

} // namespace lcocr
