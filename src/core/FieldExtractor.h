#pragma once

#include "Models.h"
#include <vector>

namespace lcocr {

class FieldExtractor {
public:
    std::vector<ExtractedField> extract(const std::string& caseId, const std::vector<PageTextLine>& unwrappedLines, const std::vector<FieldDef>& fields, const std::vector<AliasDef>& aliases) const;
private:
    std::string extractValueAfterAlias(const std::string& line, const std::string& alias) const;
};

} // namespace lcocr
