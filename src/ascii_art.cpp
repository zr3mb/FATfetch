#include "ascii_art.hpp"
#include "palettes.hpp"
#include <regex>
#include <algorithm>

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

std::vector<std::string> AsciiManager::getAvailableLogos() {
    return {"archguy", "fatfemboy", "fatarch", "discordmod", "minimal"};
}

AsciiLogo AsciiManager::getLogo(const std::string& name, bool raw, const std::string& paletteName) {
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
    std::string C_MAG = raw ? "" : "\033[1;35m";
    std::string C_PNK = raw ? "" : "\033[38;5;218m";
    std::string C_HPK = raw ? "" : "\033[38;5;198m";

    // If a custom palette is selected, override primary highlight color
    if (paletteName != "default" && !pal.ansiColors.empty() && !raw) {
        C_CYN = pal.ansiColors[0];
        if (pal.ansiColors.size() > 1) C_PNK = pal.ansiColors[1];
        if (pal.ansiColors.size() > 2) C_MAG = pal.ansiColors[2];
        if (pal.ansiColors.size() > 3) C_HPK = pal.ansiColors[3];
    }

    if (name == "fatfemboy") {
        // The legendary CHUNKY FEMBOY in programming socks & oversized Arch hoodie
        logo.lines = {
            C_PNK + "              /\\___/\\             " + C_RST,
            C_PNK + "             (  > ω < )  " + C_HPK + "♥ ♥ ♥    " + C_RST,
            C_SKN + "            /|   " + C_HPK + "///" + C_SKN + "   |\\           " + C_RST,
            C_PNK + "           ( |  " + C_HPK + "[BLUSH]" + C_PNK + "  | )          " + C_RST,
            C_BLK + "       .---'            '---.     " + C_RST,
            C_BLK + "     /     " + C_PNK + "* chonky hoodie *" + C_BLK + "  \\   " + C_RST,
            C_BLK + "    /           " + C_CYN + "/\\" + C_BLK + "              \\  " + C_RST,
            C_BLK + "   /           " + C_CYN + "/  \\" + C_BLK + "              \\ " + C_RST,
            C_BLK + "  /           " + C_CYN + "/ /\\ \\" + C_BLK + "              \\" + C_RST,
            C_BLK + " |           " + C_CYN + "/ /__\\ \\" + C_BLK + "              |" + C_RST,
            C_BLK + " |          " + C_CYN + "/_/    \\_\\" + C_BLK + "             |" + C_RST,
            C_BLK + " |          " + C_CYN + "archlinux" + C_BLK + "              |" + C_RST,
            C_BLK + " |                                 |" + C_RST,
            C_PNK + "  \\    (  300kg UWU TUMMY  )      /  " + C_RST,
            C_SKN + "   '---.___________________..--'   " + C_RST,
            C_PNK + "       /                   \\       " + C_RST,
            C_PNK + "      /  [STRIPED SOCKS]    \\      " + C_RST,
            C_CYN + "     (===)  (===)    (===)  (===)  " + C_RST,
            C_PNK + "     (===)  (===)    (===)  (===)  " + C_RST,
            C_CYN + "     (===)  (===)    (===)  (===)  " + C_RST,
            C_PNK + "    (_____) (____)  (____) (_____) " + C_RST
        };
    } else if (name == "fatarch") {
        logo.lines = {
            C_CYN + "                 /\\" + C_RST,
            C_CYN + "                /  \\" + C_RST,
            C_CYN + "               / /\\ \\" + C_RST,
            C_CYN + "              / /__\\ \\" + C_RST,
            C_CYN + "             / /    \\ \\" + C_RST,
            C_CYN + "            / /  __  \\ \\" + C_RST,
            C_CYN + "           / /  /  \\  \\ \\" + C_RST,
            C_CYN + "          / /  / /\\ \\  \\ \\" + C_RST,
            C_CYN + "        _/ /__/ /  \\ \\__\\ \\_" + C_RST,
            C_CYN + "       /____________________\\" + C_RST,
            C_WHT + "      (____   T H I C C   ____)" + C_RST,
            C_GRY + "       \\______________________/" + C_RST,
            C_MAG + "        [ B T W   I   U S E ]" + C_RST,
            C_CYN + "        [   A R C H   B T W ]" + C_RST
        };
    } else if (name == "discordmod") {
        logo.lines = {
            C_GRY + "          .--------." + C_RST,
            C_GRY + "         /  ______  \\" + C_RST,
            C_DGR + "        |  (o)  (o)  |" + C_RST,
            C_SKN + "        |     <      |" + C_RST,
            C_DGR + "        |   `----'   |" + C_RST,
            C_DGR + "         \\  ######  /" + C_RST,
            C_BLK + "      .---'########'---." + C_RST,
            C_BLK + "     /                  \\" + C_RST,
            C_BLK + "    /   " + C_CYN + "   /\\" + C_BLK + "           \\" + C_RST,
            C_BLK + "   /    " + C_CYN + "  /  \\" + C_BLK + "           \\" + C_RST,
            C_BLK + "  |     " + C_CYN + " / /\\ \\" + C_BLK + "           |" + C_RST,
            C_BLK + "  |     " + C_CYN + "archlinux" + C_BLK + "        |" + C_RST,
            C_SKN + "   \\   (  BELLY FAT )   /" + C_RST,
            C_BLK + "    '--.____________.--'" + C_RST,
            C_BLU + "        /    ||    \\" + C_RST,
            C_BLU + "       (____/  \\____)" + C_RST
        };
    } else if (name == "minimal") {
        logo.lines = {
            C_CYN + "    /\\" + C_RST,
            C_CYN + "   /  \\" + C_RST,
            C_CYN + "  / /\\ \\" + C_RST,
            C_CYN + " / /__\\ \\" + C_RST,
            C_CYN + "/________\\" + C_RST,
            C_WHT + "  [FAT]  " + C_RST
        };
    } else { // default: "archguy"
        logo.lines = {
            C_DGR + "               .---.             " + C_RST,
            C_DGR + "             /  '-. \\           " + C_RST,
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
