#include "app/Pipeline.h"
#include "io/ConfigLoader.h"
#include "io/Logger.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    try {
        fs::path root = fs::current_path();
        lcocr::Logger logger(root / "workspace" / "logs" / "run.log");

        logger.info("LC OCR CPP v0.1 started");
        logger.info("Root: " + root.u8string());

        lcocr::ConfigLoader loader;
        lcocr::AppData data = loader.load(root);

        lcocr::Pipeline pipeline(root, data, logger);
        pipeline.run();

        logger.info("Done");
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "ERROR: " << ex.what() << "\n";
        return 1;
    }
}
