#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace lcocr {
namespace fs = std::filesystem;

enum class InputFileType {
    PDF,
    Image,
    OCRText,
    Unsupported
};

struct CaseConfig {
    std::string caseId;
    fs::path inputFolder;
    bool enabled = false;
    bool runOcr = false;
    bool exportAudit = true;
};

struct FieldDef {
    std::string fieldId;
    std::string fieldName;
    bool required = false;
    std::string criticality;
};

struct AliasDef {
    std::string fieldId;
    std::string alias;
    int priority = 999;
};

struct InputDocument {
    fs::path filePath;
    InputFileType type = InputFileType::Unsupported;
};

struct OCRPage {
    std::string caseId;
    fs::path sourceFile;
    int pageNumber = 1;
    std::vector<std::string> lines;
};

struct ExtractedField {
    std::string caseId;
    std::string fieldId;
    std::string fieldName;
    std::string value;
    double confidence = 0.0;
    std::string method = "not_found";
    bool needReview = true;
    std::string sourceFile;
    int sourcePage = -1;
};

struct AppData {
    std::vector<CaseConfig> cases;
    std::vector<FieldDef> fields;
    std::vector<AliasDef> aliases;
};

} // namespace lcocr
