#include "fatjump.hpp"
#include "config.hpp"
#include <string>

int main(int argc, char* argv[]) {
    FATfetch::AppConfig cfg = FATfetch::ConfigManager::loadConfig();
    std::string pal = cfg.palette;
    std::string persona = cfg.logo;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-p" || arg == "--palette") && i + 1 < argc) {
            pal = argv[++i];
        } else if ((arg == "-l" || arg == "--logo") && i + 1 < argc) {
            persona = argv[++i];
        }
    }

    FATfetch::FatJump::run(pal, persona);
    return 0;
}
