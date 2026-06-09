#include "app/Pipeline.h"
#include "io/ConfigLoader.h"
#include "io/Logger.h"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

namespace fs = std::filesystem;

namespace {

fs::path findProjectRoot(const fs::path& startPath) {
    fs::path current = fs::absolute(startPath);
    if (fs::is_regular_file(current)) {
        current = current.parent_path();
    }

    while (true) {
        if (fs::exists(current / "config" / "Control.csv")) {
            return current;
        }

        fs::path parent = current.parent_path();
        if (parent == current || parent.empty()) {
            throw std::runtime_error("Cannot find project root from: " + startPath.u8string());
        }
        current = parent;
    }
}

void copyConfigFileWithRetry(
    const fs::path& source,
    const fs::path& destination,
    lcocr::Logger& logger,
    int retries = 3,
    std::chrono::milliseconds delay = std::chrono::milliseconds(300)) {
    std::error_code ec;
    fs::create_directories(destination.parent_path(), ec);
    if (ec) {
        throw std::runtime_error("Cannot create config snapshot folder: " + destination.parent_path().u8string());
    }

    for (int attempt = 1; attempt <= retries; ++attempt) {
        ec.clear();
        fs::copy_file(source, destination, fs::copy_options::overwrite_existing, ec);
        if (!ec) {
            logger.info("Copied config file: " + source.u8string() + " -> " + destination.u8string());
            return;
        }

        if (attempt < retries) {
            std::this_thread::sleep_for(delay);
        }
    }

    throw std::runtime_error("Cannot copy config file: " + source.u8string() + " -> "
        + destination.u8string() + " (" + ec.message() + ")");
}

fs::path createConfigSnapshot(const fs::path& root, lcocr::Logger& logger) {
    const fs::path snapshotFolder = root / "workspace" / "temp" / "config_snapshot";
    const fs::path configFolder = root / "config";
    const std::vector<std::string> configFiles = {
        "Control.csv",
        "field_dictionary.csv",
        "alias_dictionary.csv",
        "app_config.json",
    };

    logger.info("Config snapshot folder: " + snapshotFolder.u8string());
    fs::create_directories(snapshotFolder);

    for (const auto& fileName : configFiles) {
        copyConfigFileWithRetry(configFolder / fileName, snapshotFolder / fileName, logger);
    }

    return snapshotFolder;
}

} // namespace

int main() {
    try {
        fs::path root = findProjectRoot(fs::current_path());
        lcocr::Logger logger(root / "workspace" / "logs" / "run.log");

        logger.info("LC OCR CPP v0.1 started");
        logger.info("Root: " + root.u8string());

        fs::path configSnapshotFolder = createConfigSnapshot(root, logger);

        lcocr::ConfigLoader loader;
        lcocr::AppData data = loader.load(configSnapshotFolder);

        lcocr::Pipeline pipeline(root, data, logger);
        pipeline.run();

        logger.info("Done");
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "ERROR: " << ex.what() << "\n";
        return 1;
    }
}
