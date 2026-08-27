#pragma once
#include "locales.hpp"
#include <string>

namespace FATfetch {

struct AppConfig {
    std::string logo = "archguy";
    std::string palette = "default";
    Language lang = Language::PL;
    bool showJoke = true;
    bool showDiet = false;
    bool colorBlocks = true;
    bool raw = false;
};

class ConfigManager {
public:
    static std::string getConfigPath();
    static AppConfig loadConfig();
    static bool saveConfig(const AppConfig& config);
};

} // namespace FATfetch
