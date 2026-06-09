#pragma once

#include "Models.h"
#include "io/Logger.h"
#include <vector>

namespace lcocr {

class OCRRunner {
public:
    explicit OCRRunner(Logger& logger);
    std::vector<OCRPage> loadOrRunOCR(const std::string& caseId, const std::vector<InputDocument>& docs, bool runOcr, const fs::path& root);
private:
    Logger& logger_;
    OCRPage loadTextFile(const std::string& caseId, const fs::path& txtPath) const;
    std::vector<OCRPage> runExternalOCRPlaceholder(const std::string& caseId, const InputDocument& doc, const fs::path& root) const;
};

} // namespace lcocr
