#include "ascii_art.hpp"
#include "palettes.hpp"
#include <regex>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace FATfetch {

size_t AsciiManager::calculateVisibleLength(const std::string& str) {
    std::string clean = stripAnsi(str);
    size_t len = 0;
    for (size_t i = 0; i < clean.length(); ) {
        unsigned char c = clean[i];
        if (c < 0x80) {
            len += 1;
            i += 1;
        } else if ((c & 0xE0) == 0xC0) {
            len += 1;
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            len += 1;
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            len += 1;
            i += 4;
        } else {
            i += 1;
        }
    }
    return len;
}

std::string AsciiManager::stripAnsi(const std::string& str) {
    std::regex ansi_regex(R"(\x1B\[[0-9;]*[a-zA-Z])");
    return std::regex_replace(str, ansi_regex, "");
}

std::string AsciiManager::getCustomAsciiDir() {
    const char* home = getenv("HOME");
    std::string h = home ? home : "/home/user";
    return h + "/.config/fatfetch/ascii";
}

void AsciiManager::ensureCustomAsciiDir() {
    std::error_code ec;
    fs::create_directories(getCustomAsciiDir(), ec);
}

std::vector<std::string> AsciiManager::getAvailableLogos() {
    std::vector<std::string> logos = {"archguy", "fatfemboy", "fatarch", "discordmod", "minimal"};
    
    std::string customDir = getCustomAsciiDir();
    if (fs::exists(customDir) && fs::is_directory(customDir)) {
        for (const auto& entry : fs::directory_iterator(customDir)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".txt" || ext == ".ascii" || ext == ".art" || ext.empty()) {
                    std::string stem = entry.path().stem().string();
                    if (std::find(logos.begin(), logos.end(), stem) == logos.end()) {
                        logos.push_back(stem);
                    }
                }
            }
        }
    }
    return logos;
}

bool AsciiManager::importAscii(const std::string& sourcePath, const std::string& targetName) {
    if (!fs::exists(sourcePath)) return false;
    ensureCustomAsciiDir();

    std::string name = targetName;
    if (name.empty()) {
        name = fs::path(sourcePath).stem().string();
    }

    std::string dest = getCustomAsciiDir() + "/" + name + ".txt";
    std::error_code ec;
    return fs::copy_file(sourcePath, dest, fs::copy_options::overwrite_existing, ec);
}

AsciiLogo AsciiManager::loadFromFile(const std::string& filePath, bool raw, const std::string& paletteName) {
    AsciiLogo logo;
    logo.name = fs::path(filePath).stem().string();

    std::ifstream file(filePath);
    if (!file.is_open()) {
        return getLogo("archguy", raw, paletteName);
    }

    Palette pal = PaletteManager::getPalette(paletteName);

    std::string C_RST = raw ? "" : "\033[0m";
    std::string C_CYN = raw ? "" : "\033[1;36m";
    std::string C_BLU = raw ? "" : "\033[1;34m";
    std::string C_YEL = raw ? "" : "\033[1;33m";
    std::string C_GRN = raw ? "" : "\033[1;32m";
    std::string C_RED = raw ? "" : "\033[1;31m";
    std::string C_MAG = raw ? "" : "\033[1;35m";
    std::string C_WHT = raw ? "" : "\033[1;37m";
    std::string C_SKN = raw ? "" : "\033[38;5;216m";
    std::string C_BLK = raw ? "" : "\033[38;5;233m";
    std::string C_DGR = raw ? "" : "\033[38;5;236m";
    std::string C_PNK = raw ? "" : "\033[38;5;218m";

    std::string C1 = (!pal.ansiColors.empty() && !raw) ? pal.ansiColors[0] : C_CYN;
    std::string C2 = (pal.ansiColors.size() > 1 && !raw) ? pal.ansiColors[1] : C_MAG;
    std::string C3 = (pal.ansiColors.size() > 2 && !raw) ? pal.ansiColors[2] : C_BLU;
    std::string C4 = (pal.ansiColors.size() > 3 && !raw) ? pal.ansiColors[3] : C_YEL;
    std::string C5 = (pal.ansiColors.size() > 4 && !raw) ? pal.ansiColors[4] : C_GRN;
    std::string C6 = (pal.ansiColors.size() > 5 && !raw) ? pal.ansiColors[5] : C_RED;

    auto replaceAll = [](std::string& str, const std::string& from, const std::string& to) {
        if (from.empty()) return;
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    };

    std::string line;
    while (std::getline(file, line)) {
        // Skip comment lines starting with #
        if (!line.empty() && line[0] == '#') continue;

        if (raw) {
            replaceAll(line, "$C1", "");
            replaceAll(line, "$C2", "");
            replaceAll(line, "$C3", "");
            replaceAll(line, "$C4", "");
            replaceAll(line, "$C5", "");
            replaceAll(line, "$C6", "");
            replaceAll(line, "$RST", "");
            replaceAll(line, "$RESET", "");
            replaceAll(line, "$CYN", "");
            replaceAll(line, "$BLU", "");
            replaceAll(line, "$MAG", "");
            replaceAll(line, "$YEL", "");
            replaceAll(line, "$GRN", "");
            replaceAll(line, "$RED", "");
            replaceAll(line, "$WHT", "");
            replaceAll(line, "$SKN", "");
            replaceAll(line, "$BLK", "");
            replaceAll(line, "$DGR", "");
            replaceAll(line, "$PNK", "");
            line = stripAnsi(line);
        } else {
            bool hasPlaceholders = (line.find('$') != std::string::npos);
            bool hasAnsi = (line.find('\033') != std::string::npos);

            if (hasPlaceholders) {
                replaceAll(line, "$C1", C1);
                replaceAll(line, "$C2", C2);
                replaceAll(line, "$C3", C3);
                replaceAll(line, "$C4", C4);
                replaceAll(line, "$C5", C5);
                replaceAll(line, "$C6", C6);
                replaceAll(line, "$RST", C_RST);
                replaceAll(line, "$RESET", C_RST);
                replaceAll(line, "$CYN", C_CYN);
                replaceAll(line, "$BLU", C_BLU);
                replaceAll(line, "$MAG", C_MAG);
                replaceAll(line, "$YEL", C_YEL);
                replaceAll(line, "$GRN", C_GRN);
                replaceAll(line, "$RED", C_RED);
                replaceAll(line, "$WHT", C_WHT);
                replaceAll(line, "$SKN", C_SKN);
                replaceAll(line, "$BLK", C_BLK);
                replaceAll(line, "$DGR", C_DGR);
                replaceAll(line, "$PNK", C_PNK);
                line += C_RST;
            } else if (!hasAnsi) {
                // Plain ASCII - colorize automatically with palette primary color
                line = C1 + line + C_RST;
            }
        }
        logo.lines.push_back(line);
    }

    size_t maxWidth = 0;
    for (const auto& l : logo.lines) {
        size_t len = calculateVisibleLength(l);
        if (len > maxWidth) {
            maxWidth = len;
        }
    }
    logo.width = maxWidth;
    return logo;
}

AsciiLogo AsciiManager::getLogo(const std::string& name, bool raw, const std::string& paletteName) {
    // 1. Direct file path check
    if (fs::exists(name) && fs::is_regular_file(name)) {
        return loadFromFile(name, raw, paletteName);
    }

    // 2. Custom ascii directory check (~/.config/fatfetch/ascii/<name>.txt etc)
    std::string customDir = getCustomAsciiDir();
    std::vector<std::string> candidates = {
        customDir + "/" + name + ".txt",
        customDir + "/" + name + ".ascii",
        customDir + "/" + name + ".art",
        customDir + "/" + name
    };
    for (const auto& candidate : candidates) {
        if (fs::exists(candidate) && fs::is_regular_file(candidate)) {
            return loadFromFile(candidate, raw, paletteName);
        }
    }

    AsciiLogo logo;
    logo.name = name;
    Palette pal = PaletteManager::getPalette(paletteName);

    std::string C_RST = raw ? "" : "\033[0m";
    std::string C_CYN = raw ? "" : "\033[1;36m";
    std::string C_BLU = raw ? "" : "\033[1;34m";
    std::string C_GRY = raw ? "" : "\033[38;5;240m";
    std::string C_DGR = raw ? "" : "\033[38;5;236m";
    std::string C_WHT = raw ? "" : "\033[1;37m";
    std::string C_SKN = raw ? "" : "\033[38;5;216m";
    std::string C_BLK = raw ? "" : "\033[38;5;233m";
    std::string C_YEL = raw ? "" : "\033[1;33m";
    std::string C_MAG = raw ? "" : "\033[1;35m";
    std::string C_PNK = raw ? "" : "\033[38;5;218m";
    std::string C_HPK = raw ? "" : "\033[38;5;198m";

    // If a custom palette is selected, override primary highlight color
    if (paletteName != "default" && !pal.ansiColors.empty() && !raw) {
        C_CYN = pal.ansiColors[0];
        if (pal.ansiColors.size() > 1) {
            C_MAG = pal.ansiColors[1];
            C_PNK = pal.ansiColors[1];
        }
        if (pal.ansiColors.size() > 2) {
            C_BLU = pal.ansiColors[2];
        }
    }

    if (name == "fatfemboy") {
        logo.lines = {
            C_MAG + "              /\\___/\\   " + C_RST,
            C_PNK + "             (  > ω < )  ♥ ♥ ♥" + C_RST,
            C_PNK + "            /|   ///   |\\" + C_RST,
            C_PNK + "           ( |  [BLUSH]  | )" + C_RST,
            C_BLK + "       .---'            '---." + C_RST,
            C_BLK + "     /     * chonky hoodie *  \\" + C_RST,
            C_BLK + "    /           " + C_MAG + "/\\" + C_BLK + "              \\" + C_RST,
            C_BLK + "   /           " + C_MAG + "/  \\" + C_BLK + "              \\" + C_RST,
            C_BLK + "  /           " + C_MAG + "/ /\\ \\" + C_BLK + "              \\" + C_RST,
            C_BLK + " |           " + C_MAG + "/ /__\\ \\" + C_BLK + "              |" + C_RST,
            C_BLK + " |          " + C_MAG + "/_/    \\_\\" + C_BLK + "             |" + C_RST,
            C_BLK + " |          " + C_MAG + "archlinux" + C_BLK + "              |" + C_RST,
            C_BLK + " |                                 |" + C_RST,
            C_SKN + "  \\    (  300kg UWU TUMMY  )      /" + C_RST,
            C_SKN + "   '---.___________________..--'" + C_RST,
            C_WHT + "       /                   \\" + C_RST,
            C_WHT + "      /  " + C_HPK + "[STRIPED SOCKS]" + C_WHT + "    \\" + C_RST,
            C_HPK + "     (===)  (===)    (===)  (===)" + C_RST,
            C_WHT + "     (===)  (===)    (===)  (===)" + C_RST,
            C_HPK + "     (===)  (===)    (===)  (===)" + C_RST,
            C_WHT + "    (_____) (____)  (____) (_____)" + C_RST
        };
    } else if (name == "fatarch") {
        logo.lines = {
            C_CYN + "                  /\\                  " + C_RST,
            C_CYN + "                 /  \\                 " + C_RST,
            C_CYN + "                /\\   \\                " + C_RST,
            C_CYN + "               /      \\               " + C_RST,
            C_CYN + "              /   ,,   \\              " + C_RST,
            C_CYN + "             /   |  |  -\\             " + C_RST,
            C_CYN + "            /_-''    ''-_\\            " + C_RST,
            C_CYN + "          _--''  350KG  ''--_         " + C_RST,
            C_CYN + "        .                      .      " + C_RST,
            C_CYN + "       /                        \\     " + C_RST,
            C_CYN + "      |      EXTRA WIDE ARCH     |    " + C_RST,
            C_CYN + "      |                          |    " + C_RST,
            C_CYN + "     (     O B E S I T Y        )   " + C_RST,
            C_CYN + "      \\                        /    " + C_RST,
            C_CYN + "       '--__              __--'     " + C_RST,
            C_CYN + "            '''--------'''          " + C_RST
        };
    } else if (name == "discordmod") {
        logo.lines = {
            C_GRY + "           .---.           " + C_RST,
            C_SKN + "          / o.o \\       " + C_YEL + "*ZJADA BURGERA*" + C_RST,
            C_SKN + "         |  ---  |      " + C_RST,
            C_DGR + "         |  ###  |/     " + C_CYN + "*DISCORD MOD*" + C_RST,
            C_BLK + "      .---' === '--.    " + C_RST,
            C_BLK + "     /              \\   " + C_RST,
            C_BLK + "    /   " + C_SKN + "(  400KG BEBECH )" + C_BLK + " \\  " + C_RST,
            C_BLK + "   |    " + C_SKN + "(  PIWNICA EDYCJA )" + C_BLK + " | " + C_RST,
            C_BLK + "   |    " + C_SKN + "(  ZERO PRYSZNICA )" + C_BLK + " | " + C_RST,
            C_SKN + "    \\    '----------'   / " + C_RST,
            C_SKN + "     '--.____________.-'  " + C_RST,
            C_BLK + "      /     |    |    \\   " + C_RST,
            C_BLU + "     (______|    |_____)  " + C_RST
        };
    } else if (name == "minimal") {
        logo.lines = {
            C_CYN + "       /\\       " + C_RST,
            C_CYN + "      /  \\      " + C_RST,
            C_CYN + "     / /\\ \\     " + C_RST,
            C_CYN + "    / /__\\ \\    " + C_RST,
            C_CYN + "   / /    \\ \\   " + C_RST,
            C_CYN + "  /_/      \\_\\  " + C_RST,
            C_CYN + "  (  300KG ARCH ) " + C_RST
        };
    } else {
        // Default: "archguy"
        logo.lines = {
            C_DGR + "                       .---.           " + C_RST,
            C_SKN + "                      / o O \\          " + C_RST,
            C_SKN + "                      |  -  |          " + C_RST,
            C_DGR + "                      | ### |          " + C_RST,
            C_BLK + "                  .---' === '---.      " + C_RST,
            C_BLK + "                /                 \\    " + C_RST,
            C_BLK + "               /   " + C_SKN + "( ^^ 300KG ^^ )" + C_BLK + " \\   " + C_RST,
            C_BLK + "              /    " + C_SKN + "(   ARCH-CHAD )" + C_BLK + "  \\  " + C_RST,
            C_BLK + "             |     " + C_SKN + "(   BEBECH    )" + C_BLK + "   | " + C_RST,
            C_BLK + "             |     " + C_SKN + "'-------------'   " + C_BLK + "| " + C_RST,
            C_SKN + "              \\   /                 /  " + C_RST,
            C_SKN + "               '-.________________.-'   " + C_RST,
            C_SKN + "            |  " + C_BLK + ".-. .-" + C_SKN + " |           " + C_RST,
            C_SKN + "            |  " + C_BLK + "o | o" + C_SKN + "  |           " + C_RST,
            C_DGR + "            |   " + C_DGR + "###" + C_DGR + "   |           " + C_RST,
            C_DGR + "             \\  " + C_DGR + "===" + C_DGR + "  /            " + C_RST,
            C_BLK + "       .-------'     '-------.   " + C_RST,
            C_BLK + "     /                         \\ " + C_RST,
            C_BLK + "    /           " + C_CYN + "/\\" + C_BLK + "              \\ " + C_RST,
            C_BLK + "   /           " + C_CYN + "/  \\" + C_BLK + "              \\" + C_RST,
            C_BLK + "  /           " + C_CYN + "/ /\\ \\" + C_BLK + "              \\" + C_RST,
            C_BLK + " |           " + C_CYN + "/ /__\\ \\" + C_BLK + "              |" + C_RST,
            C_BLK + " |          " + C_CYN + "/_/    \\_\\" + C_BLK + "             |" + C_RST,
            C_BLK + " |          " + C_CYN + "archlinux" + C_BLK + "              |" + C_RST,
            C_BLK + " |                                 |" + C_RST,
            C_BLK + "  \\                               / " + C_RST,
            C_SKN + "   '---.___________________..--'  " + C_RST,
            C_SKN + "       (   ABSOLUTE UNIT   )      " + C_RST,
            C_BLK + "        |       |       |         " + C_RST,
            C_BLK + "        |       |       |         " + C_RST,
            C_BLU + "       /___)         (___\\        " + C_RST
        };
    }

    size_t maxWidth = 0;
    for (const auto& line : logo.lines) {
        size_t len = calculateVisibleLength(line);
        if (len > maxWidth) {
            maxWidth = len;
        }
    }
    logo.width = maxWidth;
    return logo;
}

} // namespace FATfetch
