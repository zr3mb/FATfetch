#pragma once
#include "palettes.hpp"
#include <string>
#include <vector>

namespace FATfetch {

struct AsciiLogo {
    std::string name;
    std::vector<std::string> lines;
    size_t width = 0;
};

class AsciiManager {
public:
    static AsciiLogo getLogo(const std::string& name = "archguy", bool raw = false, const std::string& paletteName = "default");
    static std::vector<std::string> getAvailableLogos();
    static size_t calculateVisibleLength(const std::string& str);
    static std::string stripAnsi(const std::string& str);
};

} // namespace FATfetch
