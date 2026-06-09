#include "io/ConfigLoader.h"
#include "io/CSV.h"

#include <algorithm>
#include <stdexcept>

namespace lcocr {

AppData ConfigLoader::load(const fs::path& configFolder) {
    AppData data;
    data.cases = loadControl(configFolder / "Control.csv");
    data.fields = loadFields(configFolder / "field_dictionary.csv");
    data.aliases = loadAliases(configFolder / "alias_dictionary.csv");
    return data;
}

std::vector<CaseConfig> ConfigLoader::loadControl(const fs::path& path) {
    auto rows = readCSV(path);
    std::vector<CaseConfig> cases;
    for (size_t i = 1; i < rows.size(); ++i) {
        if (rows[i].size() < 5) continue;
        CaseConfig c;
        c.caseId = rows[i][0];
        c.inputFolder = fs::path(rows[i][1]);
        c.enabled = rows[i][2] == "1";
        c.runOcr = rows[i][3] == "1";
        c.exportAudit = rows[i][4] == "1";
        cases.push_back(c);
    }
    return cases;
}

std::vector<FieldDef> ConfigLoader::loadFields(const fs::path& path) {
    auto rows = readCSV(path);
    std::vector<FieldDef> fields;
    for (size_t i = 1; i < rows.size(); ++i) {
        if (rows[i].size() < 4) continue;
        FieldDef f;
        f.fieldId = rows[i][0];
        f.fieldName = rows[i][1];
        f.required = rows[i][2] == "1";
        f.criticality = rows[i][3];
        fields.push_back(f);
    }
    return fields;
}

std::vector<AliasDef> ConfigLoader::loadAliases(const fs::path& path) {
    auto rows = readCSV(path);
    std::vector<AliasDef> aliases;
    for (size_t i = 1; i < rows.size(); ++i) {
        if (rows[i].size() < 3) continue;
        AliasDef a;
        a.fieldId = rows[i][0];
        a.alias = rows[i][1];
        a.priority = std::stoi(rows[i][2]);
        aliases.push_back(a);
    }
    std::sort(aliases.begin(), aliases.end(), [](const AliasDef& a, const AliasDef& b) {
        return a.priority < b.priority;
    });
    return aliases;
}

} // namespace lcocr
