#pragma once

#include "Models.h"
#include <string>
#include <vector>

namespace lcocr {

class LineUnwrapper {
public:
    std::vector<PageTextLine> unwrap(const std::vector<OCRPage>& pages, const std::vector<AliasDef>& aliases) const;
private:
    bool startsWithAlias(const std::string& line, const std::vector<AliasDef>& aliases) const;
};

} // namespace lcocr
