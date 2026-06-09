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

std::vector<PageTextLine> LineUnwrapper::unwrap(const std::vector<OCRPage>& pages, const std::vector<AliasDef>& aliases) const {
    std::vector<PageTextLine> result;
    PageTextLine buffer;
    bool hasBuffer = false;

    for (const auto& page : pages) {
        for (const auto& rawLine : page.lines) {
            std::string line = trim(rawLine.text);
            if (line.empty()) continue;

            bool isNewField = startsWithAlias(line, aliases);
            if (isNewField) {
                if (hasBuffer) result.push_back(buffer);

                buffer.text = line;
                buffer.sourceFile = rawLine.sourceFile.empty() ? page.sourceFile : rawLine.sourceFile;
                buffer.sourcePage = rawLine.sourcePage > 0 ? rawLine.sourcePage : page.pageNumber;
                hasBuffer = true;
            } else {
                if (hasBuffer) {
                    buffer.text += " " + line;
                } else {
                    buffer.text = line;
                    buffer.sourceFile = rawLine.sourceFile.empty() ? page.sourceFile : rawLine.sourceFile;
                    buffer.sourcePage = rawLine.sourcePage > 0 ? rawLine.sourcePage : page.pageNumber;
                    hasBuffer = true;
                }
            }
        }
    }

    if (hasBuffer) result.push_back(buffer);
    return result;
}

} // namespace lcocr
