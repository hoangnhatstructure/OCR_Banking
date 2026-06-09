#include "core/DocumentLoader.h"
#include "io/FileUtil.h"

#include <algorithm>
#include <stdexcept>

namespace lcocr {

InputFileType DocumentLoader::detectFileType(const fs::path& p) const {
    std::string ext = toLower(p.extension().u8string());
    if (ext == ".pdf") return InputFileType::PDF;
    if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".tif" || ext == ".tiff" || ext == ".bmp") return InputFileType::Image;
    if (ext == ".txt") return InputFileType::OCRText;
    return InputFileType::Unsupported;
}

std::vector<InputDocument> DocumentLoader::scanInputFolder(const fs::path& folder) const {
    if (!fs::exists(folder)) {
        throw std::runtime_error("Input folder does not exist: " + folder.u8string());
    }

    std::vector<InputDocument> docs;
    for (const auto& entry : fs::recursive_directory_iterator(folder)) {
        if (!entry.is_regular_file()) continue;
        InputFileType type = detectFileType(entry.path());
        if (type == InputFileType::Unsupported) continue;
        docs.push_back({ entry.path(), type });
    }

    std::sort(docs.begin(), docs.end(), [](const InputDocument& a, const InputDocument& b) {
        return a.filePath.u8string() < b.filePath.u8string();
    });

    return docs;
}

} // namespace lcocr
