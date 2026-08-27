#include "sysinfo.hpp"
#include "display.hpp"
#include "ascii_art.hpp"
#include "locales.hpp"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    FATfetch::DisplayConfig config;
    config.lang = FATfetch::LocaleManager::detectSystemLanguage();

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            FATfetch::DisplayManager::printHelp(config.lang);
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            FATfetch::DisplayManager::printVersion(config.lang);
            return 0;
        } else if (arg == "-j" || arg == "--joke") {
            config.showOnlyJoke = true;
        } else if (arg == "-d" || arg == "--diet") {
            config.showDiet = true;
        } else if (arg == "-r" || arg == "--raw") {
            config.raw = true;
        } else if ((arg == "-L" || arg == "--lang") && i + 1 < argc) {
            config.lang = FATfetch::LocaleManager::parseLanguage(argv[++i]);
        } else if (arg.rfind("--lang=", 0) == 0) {
            config.lang = FATfetch::LocaleManager::parseLanguage(arg.substr(7));
        } else if ((arg == "-l" || arg == "--logo") && i + 1 < argc) {
            config.logoName = argv[++i];
        } else if (arg.rfind("--logo=", 0) == 0) {
            config.logoName = arg.substr(7);
        } else {
            std::cerr << "Nieznana flaga / Unknown flag: " << arg << " (Use --help / Użyj --help)\n";
            return 1;
        }
    }

    FATfetch::SysInfo info = FATfetch::SysInfoCollector::collectInfo(config.lang);
    FATfetch::DisplayManager::render(info, config);

    return 0;
}
