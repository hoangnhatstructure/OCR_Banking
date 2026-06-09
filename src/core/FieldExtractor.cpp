#include "core/FieldExtractor.h"
#include "io/FileUtil.h"

#include <algorithm>
#include <cmath>

namespace lcocr {

std::string FieldExtractor::extractValueAfterAlias(const std::string& line, const std::string& alias) const {
    std::string lowerLine = toLower(line);
    std::string lowerAlias = toLower(alias);

    if (lowerLine.rfind(lowerAlias, 0) != 0) return "";

    std::string value = trim(line.substr(alias.size()));
    if (!value.empty() && value[0] == ':') value = trim(value.substr(1));
    if (!value.empty() && value[0] == '-') value = trim(value.substr(1));
    return value;
}

std::vector<ExtractedField> FieldExtractor::extract(const std::string& caseId, const std::vector<std::string>& unwrappedLines, const std::vector<FieldDef>& fields, const std::vector<AliasDef>& aliases) const {
    std::vector<ExtractedField> results;

    for (const auto& field : fields) {
        ExtractedField out;
        out.caseId = caseId;
        out.fieldId = field.fieldId;
        out.fieldName = field.fieldName;
        out.needReview = field.required;

        int bestPriority = 9999;
        for (const auto& alias : aliases) {
            if (alias.fieldId != field.fieldId) continue;

            for (const auto& line : unwrappedLines) {
                std::string value = extractValueAfterAlias(line, alias.alias);
                if (value.empty()) continue;

                if (alias.priority < bestPriority) {
                    bestPriority = alias.priority;
                    out.value = value;
                    out.method = "alias_match";
                    double aliasScore = std::max(0.0, 1.0 - 0.10 * static_cast<double>(alias.priority - 1));
                    double score = 0.85 * aliasScore;
                    if (field.criticality == "operational") score -= 0.10;
                    out.confidence = std::clamp(score, 0.0, 1.0);
                }
            }
        }

        if (!out.value.empty()) {
            out.needReview = field.required || out.confidence < 0.80;
        }
        results.push_back(out);
    }

    return results;
}

} // namespace lcocr
