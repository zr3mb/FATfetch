#include "palettes.hpp"
#include <algorithm>

namespace FATfetch {

static std::string rgbFg(int r, int g, int b) {
    return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

static std::string rgbBg(int r, int g, int b) {
    return "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

static const std::vector<Palette> PALETTES = {
    {
        "default",
        "Arch Classic Cyan",
        { rgbFg(23, 147, 209), rgbFg(51, 170, 221), rgbFg(102, 204, 255), rgbFg(180, 220, 255) },
        rgbBg(23, 147, 209) + "   " + rgbBg(51, 170, 221) + "   " + rgbBg(102, 204, 255) + "   " + rgbBg(180, 220, 255) + "   \033[0m"
    },
    {
        "femboy",
        "Femboy Pastel (Pink/White/Cyan)",
        { rgbFg(255, 182, 193), rgbFg(255, 255, 255), rgbFg(135, 206, 250), rgbFg(255, 105, 180), rgbFg(255, 218, 233) },
        rgbBg(255, 182, 193) + "   " + rgbBg(255, 255, 255) + "   " + rgbBg(135, 206, 250) + "   " + rgbBg(255, 105, 180) + "   \033[0m"
    },
    {
        "trans",
        "Trans Pride (Cyan/Pink/White)",
        { rgbFg(85, 205, 252), rgbFg(247, 168, 184), rgbFg(255, 255, 255), rgbFg(247, 168, 184), rgbFg(85, 205, 252) },
        rgbBg(85, 205, 252) + "   " + rgbBg(247, 168, 184) + "   " + rgbBg(255, 255, 255) + "   " + rgbBg(247, 168, 184) + "   " + rgbBg(85, 205, 252) + "   \033[0m"
    },
    {
        "rainbow",
        "Rainbow / Pride",
        { rgbFg(228, 3, 3), rgbFg(255, 140, 0), rgbFg(255, 237, 0), rgbFg(0, 128, 38), rgbFg(36, 64, 142), rgbFg(115, 41, 130) },
        rgbBg(228, 3, 3) + "   " + rgbBg(255, 140, 0) + "   " + rgbBg(255, 237, 0) + "   " + rgbBg(0, 128, 38) + "   " + rgbBg(36, 64, 142) + "   " + rgbBg(115, 41, 130) + "   \033[0m"
    },
    {
        "bi",
        "Bisexual Pride",
        { rgbFg(214, 2, 112), rgbFg(155, 79, 150), rgbFg(0, 56, 168) },
        rgbBg(214, 2, 112) + "    " + rgbBg(155, 79, 150) + "    " + rgbBg(0, 56, 168) + "    \033[0m"
    },
    {
        "pan",
        "Pansexual Pride",
        { rgbFg(255, 33, 140), rgbFg(255, 216, 0), rgbFg(33, 177, 255) },
        rgbBg(255, 33, 140) + "    " + rgbBg(255, 216, 0) + "    " + rgbBg(33, 177, 255) + "    \033[0m"
    },
    {
        "nonbinary",
        "Non-Binary Pride",
        { rgbFg(255, 244, 48), rgbFg(255, 255, 255), rgbFg(156, 89, 209), rgbFg(80, 80, 80) },
        rgbBg(255, 244, 48) + "   " + rgbBg(255, 255, 255) + "   " + rgbBg(156, 89, 209) + "   " + rgbBg(80, 80, 80) + "   \033[0m"
    },
    {
        "lesbian",
        "Lesbian Pride",
        { rgbFg(213, 45, 0), rgbFg(255, 154, 86), rgbFg(255, 255, 255), rgbFg(211, 98, 164), rgbFg(163, 2, 98) },
        rgbBg(213, 45, 0) + "   " + rgbBg(255, 154, 86) + "   " + rgbBg(255, 255, 255) + "   " + rgbBg(211, 98, 164) + "   " + rgbBg(163, 2, 98) + "   \033[0m"
    },
    {
        "catppuccin",
        "Catppuccin Mocha Aesthetic",
        { rgbFg(242, 205, 205), rgbFg(203, 166, 247), rgbFg(116, 199, 236), rgbFg(166, 227, 161), rgbFg(250, 179, 135) },
        rgbBg(242, 205, 205) + "   " + rgbBg(203, 166, 247) + "   " + rgbBg(116, 199, 236) + "   " + rgbBg(166, 227, 161) + "   " + rgbBg(250, 179, 135) + "   \033[0m"
    },
    {
        "dracula",
        "Dracula Cyberpunk",
        { rgbFg(189, 147, 249), rgbFg(255, 121, 198), rgbFg(139, 233, 253), rgbFg(80, 250, 123), rgbFg(241, 250, 140) },
        rgbBg(189, 147, 249) + "   " + rgbBg(255, 121, 198) + "   " + rgbBg(139, 233, 253) + "   " + rgbBg(80, 250, 123) + "   " + rgbBg(241, 250, 140) + "   \033[0m"
    }
};

const std::vector<Palette>& PaletteManager::getAllPalettes() {
    return PALETTES;
}

Palette PaletteManager::getPalette(const std::string& name) {
    std::string lower = name;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    for (const auto& pal : PALETTES) {
        if (pal.id == lower) return pal;
    }
    return PALETTES[0]; // fallback to default
}

std::vector<std::string> PaletteManager::getPaletteNames() {
    std::vector<std::string> names;
    for (const auto& pal : PALETTES) {
        names.push_back(pal.id);
    }
    return names;
}

std::string PaletteManager::colorizeLine(const std::string& line, const Palette& pal, size_t lineIndex) {
    if (pal.ansiColors.empty()) return line;
    const std::string& color = pal.ansiColors[lineIndex % pal.ansiColors.size()];
    return color + line + "\033[0m";
}

std::string PaletteManager::getPreview(const Palette& pal) {
    return pal.previewBar;
}

} // namespace FATfetch
