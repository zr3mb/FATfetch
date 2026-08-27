#pragma once
#include "palettes.hpp"
#include <string>

namespace FATfetch {

class FatJump {
public:
    static void run(const std::string& initialPalette = "default", const std::string& initialPersona = "archguy");
};

} // namespace FATfetch
