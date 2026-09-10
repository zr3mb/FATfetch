#include "fatfight.hpp"
#include "config.hpp"
#include "locales.hpp"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    FATfetch::AppConfig cfg = FATfetch::ConfigManager::loadConfig();
    std::string pal = cfg.palette;
    FATfetch::Language lang = cfg.lang;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-p" || arg == "--palette") && i + 1 < argc) {
            pal = argv[++i];
        } else if ((arg == "-L" || arg == "--lang") && i + 1 < argc) {
            lang = FATfetch::LocaleManager::parseLanguage(argv[++i]);
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "fatfight - Battle of the Bloat (Retro Terminal RPG)\n"
                      << "Użycie: fatfight [opcje]\n\n"
                      << "Opcje:\n"
                      << "  -p, --palette <nazwa>   Ustaw paletę kolorów\n"
                      << "  -L, --lang <pl|en>      Ustaw język gry\n"
                      << "  -h, --help              Wyświetl tę pomoc\n";
            return 0;
        }
    }

    FATfetch::FatFight::run(pal, lang);
    return 0;
}
