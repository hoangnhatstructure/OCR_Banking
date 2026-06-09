#include "core/OCRRunner.h"
#include "io/FileUtil.h"

#include <fstream>
#include <stdexcept>

namespace lcocr {

OCRRunner::OCRRunner(Logger& logger) : logger_(logger) {}

std::vector<OCRPage> OCRRunner::loadOrRunOCR(const std::string& caseId, const std::vector<InputDocument>& docs, bool runOcr, const fs::path& root) {
    std::vector<OCRPage> pages;

    for (const auto& doc : docs) {
        if (doc.type == InputFileType::OCRText) {
            logger_.info("Loading OCR text: " + doc.filePath.u8string());
            pages.push_back(loadTextFile(caseId, doc.filePath));
            continue;
        }

        if (!runOcr) {
            logger_.warn("Skipping non-text input because run_ocr=0: " + doc.filePath.u8string());
            continue;
        }

        auto newPages = runExternalOCRPlaceholder(caseId, doc, root);
        pages.insert(pages.end(), newPages.begin(), newPages.end());
    }

    return pages;
}

OCRPage OCRRunner::loadTextFile(const std::string& caseId, const fs::path& txtPath) const {
    OCRPage page;
    page.caseId = caseId;
    page.sourceFile = txtPath;
    page.pageNumber = 1;
    page.lines = readLines(txtPath);
    return page;
}

std::vector<OCRPage> OCRRunner::runExternalOCRPlaceholder(const std::string& caseId, const InputDocument& doc, const fs::path& root) const {
    // v0.1 placeholder:
    // This function deliberately does NOT bundle Tesseract/Poppler.
    // It creates a trace file and asks the user to add offline OCR tools later.
    // The rest of the pipeline is already ready for PDF/image inputs.
    fs::path outDir = root / "workspace" / "ocr_output" / caseId;
    ensureDirectory(outDir);

    fs::path traceFile = outDir / (safeStem(doc.filePath) + "_ocr_placeholder.txt");
    std::ofstream out(traceFile);
    out << "OCR placeholder for: " << doc.filePath.u8string() << "\n";
    out << "Add Tesseract/Poppler integration here in OCRRunner::runExternalOCRPlaceholder.\n";
    out.close();

    logger_.warn("PDF/image input detected but OCR integration is placeholder in v0.1: " + doc.filePath.u8string());
    return {};
}

} // namespace lcocr
