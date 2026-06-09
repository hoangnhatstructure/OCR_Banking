#pragma once

#include "Models.h"
#include <filesystem>

namespace lcocr {
namespace fs = std::filesystem;

class ConfigLoader {
public:
    AppData load(const fs::path& root);
private:
    std::vector<CaseConfig> loadControl(const fs::path& path);
    std::vector<FieldDef> loadFields(const fs::path& path);
    std::vector<AliasDef> loadAliases(const fs::path& path);
};

} // namespace lcocr
