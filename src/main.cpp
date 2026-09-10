#include "sysinfo.hpp"
#include "display.hpp"
#include "ascii_art.hpp"
#include "locales.hpp"
#include "palettes.hpp"
#include "config.hpp"
#include "tui_config.hpp"
#include "fatjump.hpp"
#include "discord_rpc.hpp"
#include "fatgotchi.hpp"
#include "fatrain.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

int main(int argc, char* argv[]) {
    // 1. Load config file from ~/.config/fatfetch/config.conf
    FATfetch::AppConfig fileConfig = FATfetch::ConfigManager::loadConfig();

    FATfetch::DisplayConfig config;
    config.logoName = fileConfig.logo;
    config.paletteName = fileConfig.palette;
    config.lang = fileConfig.lang;
    config.showJoke = fileConfig.showJoke;
    config.showDiet = fileConfig.showDiet;
    config.showColorBlocks = fileConfig.colorBlocks;
    config.raw = fileConfig.raw;

    // 2. Parse CLI flags (CLI flags override config file)
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-c" || arg == "--config" || arg == "config") {
            FATfetch::TuiConfigurator::run();
            return 0;
        } else if (arg == "--game" || arg == "game" || arg == "--gotchi" || arg == "gotchi") {
            FATfetch::FatgotchiGame::run();
            return 0;
        } else if (arg == "--rain" || arg == "rain") {
            FATfetch::FatRain::run(config.paletteName);
            return 0;
        } else if (arg == "--jump" || arg == "jump") {
            FATfetch::FatJump::run(config.paletteName, config.logoName);
            return 0;
        } else if (arg == "--rpc" || arg == "rpc") {
            FATfetch::DiscordRPC::runDaemon(true);
            return 0;
        } else if (arg == "--rpc-daemon") {
            FATfetch::DiscordRPC::runDaemon(false);
            return 0;
        } else if (arg == "--rpc-enable") {
            FATfetch::DiscordRPC::enableAutostart();
            std::cout << "\033[1;32m✔ Pomyślnie włączono autostart Discord Rich Presence!\033[0m\n";
            return 0;
        } else if (arg == "--rpc-disable") {
            FATfetch::DiscordRPC::disableAutostart();
            std::cout << "\033[1;33m✔ Wyłączono autostart Discord Rich Presence.\033[0m\n";
            return 0;
        } else if (arg == "--rpc-status") {
            FATfetch::DiscordRPC::printStatus();
            return 0;
        } else if (arg == "-h" || arg == "--help") {
            FATfetch::DisplayManager::printHelp(config.lang);
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            FATfetch::DisplayManager::printVersion(config.lang);
            return 0;
        } else if (arg == "--list-palettes") {
            FATfetch::DisplayManager::listPalettes();
            return 0;
        } else if (arg == "--list-logos") {
            FATfetch::DisplayManager::listLogos();
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
        } else if ((arg == "-p" || arg == "--palette") && i + 1 < argc) {
            config.paletteName = argv[++i];
        } else if (arg.rfind("--palette=", 0) == 0) {
            config.paletteName = arg.substr(10);
        } else if ((arg == "-l" || arg == "--logo") && i + 1 < argc) {
            config.logoName = argv[++i];
        } else if (arg.rfind("--logo=", 0) == 0) {
            config.logoName = arg.substr(7);
        } else if ((arg == "-a" || arg == "--ascii") && i + 1 < argc) {
            config.logoName = argv[++i];
        } else if (arg.rfind("--ascii=", 0) == 0) {
            config.logoName = arg.substr(8);
        } else if (arg == "--import-ascii" && i + 1 < argc) {
            std::string src = argv[++i];
            std::string name = (i + 1 < argc && argv[i + 1][0] != '-') ? argv[++i] : "";
            if (FATfetch::AsciiManager::importAscii(src, name)) {
                std::string targetName = name.empty() ? std::filesystem::path(src).stem().string() : name;
                std::cout << "\033[1;32m✔ Pomyślnie zaimportowano postać ASCII: \033[1;37m" << targetName << "\033[1;32m do katalogu: \033[1;37m"
                          << FATfetch::AsciiManager::getCustomAsciiDir() << "\033[0m\n"
                          << "Możesz jej natychmiast użyć wpisując: \033[1;36mfatfetch -l " << targetName << "\033[0m\n";
            } else {
                std::cerr << "\033[1;31m✖ Błąd: Nie znaleziono pliku źródłowego: " << src << "\033[0m\n";
                return 1;
            }
            return 0;
        } else {
            std::cerr << "Nieznana flaga / Unknown flag: " << arg << " (Use --help / Użyj --help)\n";
            return 1;
        }
    }

    FATfetch::SysInfo info = FATfetch::SysInfoCollector::collectInfo(config.lang);
    FATfetch::DisplayManager::render(info, config);

    return 0;
}
