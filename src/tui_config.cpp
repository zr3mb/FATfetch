#include "tui_config.hpp"
#include "palettes.hpp"
#include "ascii_art.hpp"
#include "display.hpp"
#include "sysinfo.hpp"
#include "discord_rpc.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <unistd.h>

namespace FATfetch {

static void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void TuiConfigurator::run() {
    AppConfig cfg = ConfigManager::loadConfig();

    std::vector<std::string> logos = AsciiManager::getAvailableLogos();
    std::vector<std::string> palettes = PaletteManager::getPaletteNames();

    size_t logoIndex = 0;
    for (size_t i = 0; i < logos.size(); ++i) {
        if (logos[i] == cfg.logo) {
            logoIndex = i;
            break;
        }
    }

    size_t paletteIndex = 0;
    for (size_t i = 0; i < palettes.size(); ++i) {
        if (palettes[i] == cfg.palette) {
            paletteIndex = i;
            break;
        }
    }

    while (true) {
        clearScreen();
        std::string C_RST = "\033[0m";
        std::string C_CYN = "\033[1;36m";
        std::string C_MAG = "\033[1;35m";
        std::string C_YEL = "\033[1;33m";
        std::string C_GRN = "\033[1;32m";
        std::string C_WHT = "\033[1;37m";

        std::cout << C_CYN
                  << "╔════════════════════════════════════════════════════════════════════════════════╗\n"
                  << "║              FATfetch - INTERAKTYWNY GRAFICZNY KONFIGURATOR TUI                ║\n"
                  << "║         Dostosuj grubego femboya, kolory LGBT/Hyfetch i Discord RPC            ║\n"
                  << "╚════════════════════════════════════════════════════════════════════════════════╝\n"
                  << C_RST << "\n";

        Palette currentPal = PaletteManager::getPalette(palettes[paletteIndex]);
        std::string palPreview = PaletteManager::getPreview(currentPal);
        bool rpcEnabled = DiscordRPC::isAutostartEnabled();

        std::cout << C_YEL << "[ AKTUALNA KONFIGURACJA / LIVE PREVIEW ]" << C_RST << "\n";
        std::cout << " 📁 Plik: " << C_WHT << ConfigManager::getConfigPath() << C_RST << "\n\n";

        // Display current settings
        std::cout << "  1) Logo ASCII:        " << C_CYN << "[" << logos[logoIndex] << "]" << C_RST << "  (Naciśnij 1, aby zmienić)\n";
        std::cout << "  2) Paleta / Pride:    " << C_MAG << "[" << currentPal.name << "]" << C_RST << " " << palPreview << "  (Naciśnij 2)\n";
        std::cout << "  3) Język / Language:  " << C_WHT << "[" << (cfg.lang == Language::PL ? "Polski 🇵🇱" : "English 🇺🇸") << "]" << C_RST << "  (Naciśnij 3)\n";
        std::cout << "  4) Losowy żart:       " << (cfg.showJoke ? C_GRN + "[WŁĄCZONY]" : "\033[1;31m[WYŁĄCZONY]") << C_RST << "  (Naciśnij 4)\n";
        std::cout << "  5) Tabela diety:      " << (cfg.showDiet ? C_GRN + "[WŁĄCZONA]" : "\033[1;31m[WYŁĄCZONA]") << C_RST << "  (Naciśnij 5)\n";
        std::cout << "  6) Kolorowe klocki:   " << (cfg.colorBlocks ? C_GRN + "[WŁĄCZONE]" : "\033[1;31m[WYŁĄCZONE]") << C_RST << "  (Naciśnij 6)\n";
        std::cout << "  7) Discord RPC:       " << (rpcEnabled ? C_GRN + "[AUTOSTART AKTYWNY]" : "\033[1;31m[WYŁĄCZONY]") << C_RST << "  (Naciśnij 7, aby przełączyć)\n\n";

        std::cout << " ────────────────────────────────────────────────────────────────────────────────\n";
        std::cout << "  8) " << C_GRN << "💾 ZAPISZ I ZASTOSUJ USTAWIENIA" << C_RST << "\n";
        std::cout << "  9) " << C_CYN << "🚀 PODGLĄD NA ŻYWO (Uruchom FATfetch)" << C_RST << "\n";
        std::cout << "  0) " << C_WHT << "❌ Wyjdź bez zapisywania" << C_RST << "\n";
        std::cout << " ────────────────────────────────────────────────────────────────────────────────\n\n";

        // Mini preview of the selected logo
        std::cout << C_YEL << "[ PODGLĄD LOGO: " << logos[logoIndex] << " ]" << C_RST << "\n";
        AsciiLogo previewLogo = AsciiManager::getLogo(logos[logoIndex], false, palettes[paletteIndex]);
        size_t maxLines = std::min(previewLogo.lines.size(), (size_t)8);
        for (size_t i = 0; i < maxLines; ++i) {
            std::cout << "  " << previewLogo.lines[i] << "\n";
        }
        if (previewLogo.lines.size() > maxLines) {
            std::cout << "  ...\n";
        }

        std::cout << "\n" << C_WHT << "Wybierz opcję [0-9]: " << C_RST;
        std::string input;
        std::getline(std::cin, input);

        if (input == "1") {
            logoIndex = (logoIndex + 1) % logos.size();
            cfg.logo = logos[logoIndex];
        } else if (input == "2") {
            paletteIndex = (paletteIndex + 1) % palettes.size();
            cfg.palette = palettes[paletteIndex];
        } else if (input == "3") {
            cfg.lang = (cfg.lang == Language::PL) ? Language::EN : Language::PL;
        } else if (input == "4") {
            cfg.showJoke = !cfg.showJoke;
        } else if (input == "5") {
            cfg.showDiet = !cfg.showDiet;
        } else if (input == "6") {
            cfg.colorBlocks = !cfg.colorBlocks;
        } else if (input == "7") {
            if (DiscordRPC::isAutostartEnabled()) {
                DiscordRPC::disableAutostart();
            } else {
                DiscordRPC::enableAutostart();
            }
        } else if (input == "8" || input == "s" || input == "S" || input == "zapisz") {
            cfg.logo = logos[logoIndex];
            cfg.palette = palettes[paletteIndex];
            if (ConfigManager::saveConfig(cfg)) {
                clearScreen();
                std::cout << "\n\033[1;32m✔ Pomyślnie zapisano konfigurację w: \033[1;37m" << ConfigManager::getConfigPath() << "\033[0m\n\n";
                sleep(1);
            }
            break;
        } else if (input == "9" || input == "r" || input == "R") {
            clearScreen();
            DisplayConfig disp;
            disp.logoName = logos[logoIndex];
            disp.paletteName = palettes[paletteIndex];
            disp.lang = cfg.lang;
            disp.showOnlyJoke = false;
            disp.showDiet = cfg.showDiet;
            disp.showColorBlocks = cfg.colorBlocks;
            disp.showJoke = cfg.showJoke;

            SysInfo info = SysInfoCollector::collectInfo(cfg.lang);
            DisplayManager::render(info, disp);

            std::cout << "\n\033[1;33mNaciśnij [ENTER], aby wrócić do konfiguratora...\033[0m";
            std::string d;
            std::getline(std::cin, d);
        } else if (input == "0" || input == "q" || input == "Q" || input == "exit") {
            break;
        }
    }
}

} // namespace FATfetch
