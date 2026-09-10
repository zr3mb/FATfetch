#pragma once
#include "locales.hpp"
#include <string>

namespace FATfetch {

class FatFight {
public:
    static void run(const std::string& initialPalette = "default", Language lang = Language::PL);
};

} // namespace FATfetch
