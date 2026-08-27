#pragma once
#include "sysinfo.hpp"
#include "ascii_art.hpp"
#include "locales.hpp"
#include <string>

namespace FATfetch {

struct DisplayConfig {
    std::string logoName = "archguy";
    Language lang = Language::PL;
    bool raw = false;
    bool showOnlyJoke = false;
    bool showDiet = false;
};

class DisplayManager {
public:
    static void render(const SysInfo& info, const DisplayConfig& config);
    static void renderJokeOnly(Language lang, bool raw = false);
    static void renderDietOnly(Language lang, bool raw = false);
    static void printHelp(Language lang = Language::PL);
    static void printVersion(Language lang = Language::PL);
};

} // namespace FATfetch
