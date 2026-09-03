#include "display.hpp"
#include "jokes.hpp"
#include "locales.hpp"
#include "palettes.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

namespace FATfetch {

void DisplayManager::printHelp(Language lang) {
    std::cout << LocaleManager::getHelpText(lang);
    std::cout << "\n\033[1;35mKONFIGURACJA, GRY, WIDŻETY I DISCORD RPC:\033[0m\n"
              << "  -c, --config          Uruchom interaktywny graficzny konfigurator TUI\n"
              << "  --game                Uruchom grę 'fatgotchi' (karm burgerami, nawadniaj Monsterem!)\n"
              << "  --jump                Uruchom widżet 'fatjump' (grubas skaczący w zwolnionym tempie)\n"
              << "  --rpc                 Uruchom Discord Rich Presence w trybie podglądu na żywo\n"
              << "  --rpc-enable          Włącz autostart demona Discord RPC przy starcie systemu\n"
              << "  --rpc-disable         Wyłącz autostart Discord RPC\n"
              << "  --rpc-status          Sprawdź status demona Discord RPC i połączenia\n"
              << "  -p, --palette <nazwa> Wybierz paletę kolorów: femboy, trans, rainbow, bi, pan, nonbinary, lesbian, catppuccin, dracula\n"
              << "  --list-palettes       Wyświetl listę dostępnych palet i motywów kolorystycznych\n"
              << "  --list-logos          Wyświetl listę dostępnych postaci i logotypów ASCII\n\n";
}

void DisplayManager::printVersion(Language lang) {
    std::cout << LocaleManager::getVersionText(lang);
}

void DisplayManager::listPalettes() {
    std::cout << "\n\033[1;36m[ DOSTĘPNE PALETY KOLORÓW / PRIDE & AESTHETICS ]\033[0m\n\n";
    for (const auto& pal : PaletteManager::getAllPalettes()) {
        std::cout << "  • \033[1m" << pal.id << "\033[0m (" << pal.name << "):\n"
                  << "    " << pal.previewBar << "\n\n";
    }
}

void DisplayManager::listLogos() {
    std::cout << "\n\033[1;36m[ DOSTĘPNE POSTACIE I LOGA ASCII ]\033[0m\n\n";
    std::cout << "  1) \033[1marchguy\033[0m      - Legendarny gość w koszulce Arch Linux (Domyślne)\n"
              << "  2) \033[1mfatfemboy\033[0m    - Gruby femboy w zakolanówkach i bluzie Arch Linux 300kg UwU\n"
              << "  3) \033[1mfatarch\033[0m      - Ultra-szerokie, spasiony logo Archa /\\\n"
              << "  4) \033[1mdiscordmod\033[0m   - Mod z piwnicy\n"
              << "  5) \033[1mminimal\033[0m      - Małe logo dla małych terminali\n\n";
}

void DisplayManager::renderJokeOnly(Language lang, bool raw) {
    std::string joke = JokeGenerator::getRandomJoke(lang);
    if (raw) {
        std::cout << joke << "\n";
    } else {
        std::string title = (lang == Language::PL) ? "[ ŻART O UŻYTKOWNIKACH ARCHA ]" : "[ ARCH LINUX JOKE OF THE DAY ]";
        std::cout << "\n\033[1;33m" << title << "\033[0m\n"
                  << "\033[1;37m" << joke << "\033[0m\n\n";
    }
}

void DisplayManager::renderDietOnly(Language lang, bool raw) {
    std::string diet = JokeGenerator::getDietPlan(lang);
    if (raw) {
        std::cout << AsciiManager::stripAnsi(diet) << "\n";
    } else {
        std::cout << "\n" << diet << "\n";
    }
}

void DisplayManager::render(const SysInfo& info, const DisplayConfig& config) {
    if (config.showOnlyJoke) {
        renderJokeOnly(config.lang, config.raw);
        return;
    }
    if (config.showDiet) {
        renderDietOnly(config.lang, config.raw);
        return;
    }

    AsciiLogo logo = AsciiManager::getLogo(config.logoName, config.raw, config.paletteName);
    Palette pal = PaletteManager::getPalette(config.paletteName);

    // Color definitions
    std::string C_RST = config.raw ? "" : "\033[0m";
    std::string C_BLD = config.raw ? "" : "\033[1m";
    std::string C_CYN = config.raw ? "" : "\033[1;36m";
    std::string C_MAG = config.raw ? "" : "\033[1;35m";
    std::string C_BLU = config.raw ? "" : "\033[1;34m";
    std::string C_YEL = config.raw ? "" : "\033[1;33m";
    std::string C_GRN = config.raw ? "" : "\033[1;32m";
    std::string C_RED = config.raw ? "" : "\033[1;31m";
    std::string C_WHT = config.raw ? "" : "\033[1;37m";
    std::string C_GRY = config.raw ? "" : "\033[38;5;244m";

    if (config.paletteName != "default" && !pal.ansiColors.empty() && !config.raw) {
        C_CYN = pal.ansiColors[0];
        if (pal.ansiColors.size() > 1) C_MAG = pal.ansiColors[1];
        if (pal.ansiColors.size() > 2) C_BLU = pal.ansiColors[2];
        if (pal.ansiColors.size() > 3) C_GRN = pal.ansiColors[3];
    }

    // Title formatting
    std::string titleStr = info.userHostTitle;
    if (config.logoName == "fatfemboy") {
        titleStr = info.username + "@" + info.hostname + " [Chonky Femboy UwU]";
    }

    // Prepare info lines
    std::vector<std::string> infoLines;
    infoLines.push_back(C_CYN + C_BLD + titleStr + C_RST);
    infoLines.push_back(C_GRY + std::string(titleStr.length() > 50 ? 50 : titleStr.length(), '-') + C_RST);

    auto addField = [&](const std::string& labelColor, const std::string& label, const std::string& val) {
        infoLines.push_back(labelColor + C_BLD + label + ": " + C_RST + val);
    };

    addField(C_CYN, info.labels.os, info.os);
    addField(C_BLU, info.labels.host, info.host);
    addField(C_MAG, info.labels.kernel, info.kernel);
    addField(C_GRN, info.labels.uptime, info.uptime);
    addField(C_YEL, info.labels.packages, info.packages);
    addField(C_CYN, info.labels.shell, info.shell);
    addField(C_MAG, info.labels.resolution, info.resolution);
    addField(C_BLU, info.labels.de_wm, info.de_wm);
    addField(C_GRN, info.labels.theme, info.theme);
    addField(C_YEL, info.labels.terminal, info.terminal);
    addField(C_CYN, info.labels.cpu, info.cpu);
    addField(C_MAG, info.labels.gpu, info.gpu);
    addField(C_RED, info.labels.memory, info.memory);
    addField(C_BLU, info.labels.disk, info.disk);
    addField(C_GRN, info.labels.grass, info.grass);
    addField(C_YEL, info.labels.shower, info.shower);
    addField(C_RED, info.labels.girlfriend, info.girlfriend);

    // Add empty line and color blocks
    if (config.showColorBlocks) {
        infoLines.push_back("");
        if (!config.raw) {
            if (config.paletteName != "default") {
                infoLines.push_back(pal.previewBar);
            } else {
                std::string colorBlocks = 
                    "\033[40m   \033[41m   \033[42m   \033[43m   \033[44m   \033[45m   \033[46m   \033[47m   \033[0m";
                std::string lightBlocks = 
                    "\033[100m   \033[101m   \033[102m   \033[103m   \033[104m   \033[105m   \033[106m   \033[107m   \033[0m";
                infoLines.push_back(colorBlocks);
                infoLines.push_back(lightBlocks);
            }
        } else {
            infoLines.push_back("[███][███][███][███][███][███][███][███]");
        }
    }

    // Zip logo.lines and infoLines side by side
    size_t totalLines = std::max(logo.lines.size(), infoLines.size());
    size_t logoPadding = logo.width + 4;

    std::cout << "\n";
    for (size_t i = 0; i < totalLines; ++i) {
        std::string logoLine = (i < logo.lines.size()) ? logo.lines[i] : "";
        std::string infoLine = (i < infoLines.size()) ? infoLines[i] : "";

        size_t visLen = AsciiManager::calculateVisibleLength(logoLine);
        size_t spaceNeeded = (logoPadding > visLen) ? (logoPadding - visLen) : 2;

        std::cout << logoLine << std::string(spaceNeeded, ' ') << infoLine << "\n";
    }

    // Print randomized Arch Joke in banner if enabled
    if (config.showJoke) {
        if (!config.raw) {
            std::string bannerTitle = "┌─[ " + info.labels.jokeBannerTitle + " ]";
            size_t padLen = (bannerTitle.length() < 70) ? (70 - bannerTitle.length()) : 5;
            std::cout << "\n " << C_YEL << C_BLD << bannerTitle << std::string(padLen, '-') << "┐" << C_RST << "\n";
            std::cout << " " << C_YEL << "│" << C_RST << " " << C_WHT << info.archQuote << C_RST << "\n";
            std::cout << " " << C_YEL << C_BLD << "└" << std::string(66, '-') << "┘" << C_RST << "\n\n";
        } else {
            std::cout << "\n--- " << info.labels.jokeBannerTitle << " ---\n" << info.archQuote << "\n\n";
        }
    }
}

} // namespace FATfetch
