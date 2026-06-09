#pragma once

#include "Models.h"
#include "io/Logger.h"

namespace lcocr {

class Pipeline {
public:
    Pipeline(const fs::path& root, const AppData& data, Logger& logger);
    void run();
private:
    fs::path root_;
    const AppData& data_;
    Logger& logger_;
    void processCase(const CaseConfig& c);
    void writeAuditCSV(const fs::path& outputPath, const std::vector<ExtractedField>& fields) const;
    void writeCombinedOCRText(const fs::path& outputPath, const std::vector<OCRPage>& pages) const;
};

} // namespace lcocr
