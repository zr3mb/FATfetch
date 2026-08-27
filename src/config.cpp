#include "config.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdlib>
#include <algorithm>

namespace fs = std::filesystem;

namespace FATfetch {

static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n\"");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n\"");
    return str.substr(first, (last - first + 1));
}

std::string ConfigManager::getConfigPath() {
    const char* xdgConfig = std::getenv("XDG_CONFIG_HOME");
    std::string base;
    if (xdgConfig && *xdgConfig) {
        base = xdgConfig;
    } else {
        const char* home = std::getenv("HOME");
        base = home ? (std::string(home) + "/.config") : "/home/user/.config";
    }
    return base + "/fatfetch/config.conf";
}

AppConfig ConfigManager::loadConfig() {
    AppConfig config;
    config.lang = LocaleManager::detectSystemLanguage();

    std::string path = getConfigPath();
    if (!fs::exists(path)) {
        return config;
    }

    std::ifstream file(path);
    if (!file.is_open()) return config;

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;

        std::string key = trim(line.substr(0, eq));
        std::string val = trim(line.substr(eq + 1));
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);

        if (key == "logo") {
            config.logo = val;
        } else if (key == "palette") {
            config.palette = val;
        } else if (key == "lang") {
            config.lang = LocaleManager::parseLanguage(val);
        } else if (key == "show_joke") {
            config.showJoke = (val == "true" || val == "1" || val == "yes");
        } else if (key == "show_diet") {
            config.showDiet = (val == "true" || val == "1" || val == "yes");
        } else if (key == "color_blocks") {
            config.colorBlocks = (val == "true" || val == "1" || val == "yes");
        } else if (key == "raw") {
            config.raw = (val == "true" || val == "1" || val == "yes");
        }
    }

    return config;
}

bool ConfigManager::saveConfig(const AppConfig& config) {
    std::string path = getConfigPath();
    std::error_code ec;
    fs::create_directories(fs::path(path).parent_path(), ec);

    std::ofstream file(path);
    if (!file.is_open()) return false;

    file << "# ========================================================\n"
         << "#  🍔 FATfetch Configuration File\n"
         << "# ========================================================\n\n"
         << "# Logo preset: archguy, fatfemboy, fatarch, discordmod, minimal\n"
         << "logo = " << config.logo << "\n\n"
         << "# Color palette / Pride theme: default, femboy, trans, rainbow, bi, pan, nonbinary, lesbian, catppuccin, dracula\n"
         << "palette = " << config.palette << "\n\n"
         << "# Language: pl, en\n"
         << "lang = " << LocaleManager::languageToString(config.lang) << "\n\n"
         << "# Show randomized Arch Linux meme joke on run\n"
         << "show_joke = " << (config.showJoke ? "true" : "false") << "\n\n"
         << "# Show 4500 kcal official Arch Chad diet plan\n"
         << "show_diet = " << (config.showDiet ? "true" : "false") << "\n\n"
         << "# Show bottom color palette blocks\n"
         << "color_blocks = " << (config.colorBlocks ? "true" : "false") << "\n\n"
         << "# Output plain raw text without ANSI colors\n"
         << "raw = " << (config.raw ? "true" : "false") << "\n";

    return true;
}

} // namespace FATfetch
