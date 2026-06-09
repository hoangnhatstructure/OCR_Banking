#include "app/Pipeline.h"
#include "core/DocumentLoader.h"
#include "core/OCRRunner.h"
#include "core/LineUnwrapper.h"
#include "core/FieldExtractor.h"
#include "io/FileUtil.h"

#include <fstream>
#include <stdexcept>

namespace lcocr {

Pipeline::Pipeline(const fs::path& root, const AppData& data, Logger& logger)
    : root_(root), data_(data), logger_(logger) {}

void Pipeline::run() {
    for (const auto& c : data_.cases) {
        if (!c.enabled) continue;
        processCase(c);
    }
}

void Pipeline::processCase(const CaseConfig& c) {
    logger_.info("Processing case: " + c.caseId);

    fs::path inputFolder = root_ / c.inputFolder;

    DocumentLoader loader;
    OCRRunner ocr(logger_);
    LineUnwrapper unwrapper;
    FieldExtractor extractor;

    auto docs = loader.scanInputFolder(inputFolder);
    logger_.info("Input documents found: " + std::to_string(docs.size()));

    auto pages = ocr.loadOrRunOCR(c.caseId, docs, c.runOcr, root_);
    logger_.info("OCR pages loaded: " + std::to_string(pages.size()));

    fs::path ocrOut = root_ / "workspace" / "ocr_output" / c.caseId / "combined_ocr_text.txt";
    writeCombinedOCRText(ocrOut, pages);

    auto unwrapped = unwrapper.unwrap(pages, data_.aliases);
    auto fields = extractor.extract(c.caseId, unwrapped, data_.fields, data_.aliases);

    if (c.exportAudit) {
        fs::path auditFile = root_ / "workspace" / "output_audit" / (c.caseId + "_audit.csv");
        writeAuditCSV(auditFile, fields);
        logger_.info("Audit exported: " + auditFile.u8string());
    }
}

void Pipeline::writeCombinedOCRText(const fs::path& outputPath, const std::vector<OCRPage>& pages) const {
    ensureDirectory(outputPath.parent_path());
    std::ofstream file(outputPath);
    if (!file.is_open()) throw std::runtime_error("Cannot write OCR text: " + outputPath.u8string());

    for (const auto& page : pages) {
        file << "===== SOURCE: " << page.sourceFile.u8string() << " PAGE: " << page.pageNumber << " =====\n";
        for (const auto& line : page.lines) file << line.text << "\n";
        file << "\n";
    }
}

void Pipeline::writeAuditCSV(const fs::path& outputPath, const std::vector<ExtractedField>& fields) const {
    ensureDirectory(outputPath.parent_path());
    std::ofstream file(outputPath);
    if (!file.is_open()) throw std::runtime_error("Cannot write audit file: " + outputPath.u8string());

    file << "case_id,field_id,field_name,value,confidence,method,need_review,source_file,source_page\n";
    for (const auto& f : fields) {
        file << csvEscape(f.caseId) << ","
             << csvEscape(f.fieldId) << ","
             << csvEscape(f.fieldName) << ","
             << csvEscape(f.value) << ","
             << f.confidence << ","
             << csvEscape(f.method) << ","
             << (f.needReview ? "Yes" : "No") << ","
             << csvEscape(f.sourceFile) << ","
             << f.sourcePage << "\n";
    }
}

} // namespace lcocr
