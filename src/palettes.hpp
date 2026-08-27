#pragma once
#include <string>
#include <vector>

namespace FATfetch {

struct Palette {
    std::string id;
    std::string name;
    std::vector<std::string> ansiColors; // ANSI 24-bit or 256 color escape codes
    std::string previewBar;
};

class PaletteManager {
public:
    static const std::vector<Palette>& getAllPalettes();
    static Palette getPalette(const std::string& name);
    static std::string colorizeLine(const std::string& line, const Palette& pal, size_t lineIndex);
    static std::string getPreview(const Palette& pal);
    static std::vector<std::string> getPaletteNames();
};

} // namespace FATfetch
