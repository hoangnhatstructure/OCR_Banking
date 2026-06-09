#pragma once

#include "Models.h"
#include <vector>

namespace lcocr {

class DocumentLoader {
public:
    std::vector<InputDocument> scanInputFolder(const fs::path& folder) const;
private:
    InputFileType detectFileType(const fs::path& p) const;
};

} // namespace lcocr
