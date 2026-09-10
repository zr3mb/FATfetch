#include "fatrain.hpp"
#include "config.hpp"

int main(int argc, char* argv[]) {
    FATfetch::AppConfig cfg = FATfetch::ConfigManager::loadConfig();
    std::string palette = cfg.palette;
    if (argc > 1) {
        palette = argv[1];
    }
    FATfetch::FatRain::run(palette);
    return 0;
}
