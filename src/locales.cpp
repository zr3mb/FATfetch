#include "locales.hpp"
#include <cstdlib>
#include <sstream>
#include <algorithm>

namespace FATfetch {

Language LocaleManager::detectSystemLanguage() {
    const char* lcAll = std::getenv("LC_ALL");
    const char* lcMsg = std::getenv("LC_MESSAGES");
    const char* lang = std::getenv("LANG");

    std::string envLang = "";
    if (lcAll && *lcAll) envLang = lcAll;
    else if (lcMsg && *lcMsg) envLang = lcMsg;
    else if (lang && *lang) envLang = lang;

    if (envLang.rfind("pl", 0) == 0 || envLang.rfind("PL", 0) == 0) {
        return Language::PL;
    }
    return Language::EN;
}

Language LocaleManager::parseLanguage(const std::string& langStr) {
    std::string lower = langStr;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "pl" || lower == "polish" || lower == "polski") {
        return Language::PL;
    }
    return Language::EN;
}

std::string LocaleManager::languageToString(Language lang) {
    return (lang == Language::PL) ? "pl" : "en";
}

FieldLabels LocaleManager::getLabels(Language lang) {
    if (lang == Language::PL) {
        return {
            "OS",
            "Sprzęt",
            "Kernel",
            "Czas pracy",
            "Pakiety",
            "Powłoka",
            "Ekran",
            "WM/DE",
            "Motyw",
            "Terminal",
            "Procesor",
            "Grafika",
            "Pamięć RAM",
            "Dysk (/)",
            "Trawa",
            "Prysznic",
            "Dziewczyna",
            "ŻART O UŻYTKOWNIKACH ARCHA"
        };
    } else {
        return {
            "OS",
            "Host",
            "Kernel",
            "Uptime",
            "Packages",
            "Shell",
            "Resolution",
            "WM/DE",
            "Theme",
            "Terminal",
            "CPU",
            "GPU",
            "Memory",
            "Disk (/)",
            "Grass",
            "Shower",
            "Girlfriend",
            "ARCH LINUX JOKE OF THE RUN"
        };
    }
}

std::string LocaleManager::getTitleSuffix(Language lang) {
    return (lang == Language::PL) ? "[Arch GigaKozak]" : "[Arch GigaChad]";
}

std::string LocaleManager::getHostSuffix(Language lang) {
    return (lang == Language::PL) ? "[Wersja Pancerz 6XL]" : "[Heavy Duty Edition]";
}

std::string LocaleManager::getKernelSuffix(Language lang) {
    return (lang == Language::PL) ? "(własny zen bez zbędnego bloatu)" : "(custom bloat-free zen)";
}

std::string LocaleManager::getUptimeSuffix(Language lang) {
    return (lang == Language::PL) ? "(zero restartów, zero prysznica)" : "(no reboots, no showers)";
}

std::string LocaleManager::getPackagesSuffix(Language lang, size_t count, size_t flatpaks) {
    std::ostringstream ss;
    if (lang == Language::PL) {
        ss << count << " (pacman/AUR)";
        if (flatpaks > 0) {
            ss << ", " << flatpaks << " (flatpakowy bloat)";
        } else {
            ss << " (0 flatpaków - bloat odrzucony)";
        }
    } else {
        ss << count << " (pacman/AUR)";
        if (flatpaks > 0) {
            ss << ", " << flatpaks << " (flatpak bloat)";
        } else {
            ss << " (0 flatpaks - pure bloat rejection)";
        }
    }
    return ss.str();
}

std::string LocaleManager::getShellSuffix(Language lang) {
    return (lang == Language::PL) 
        ? "(z 42 powolnymi wtyczkami opóźniającymi start o 4s)" 
        : "(with 42 slow starship/p10k plugins)";
}

std::string LocaleManager::getResolutionSuffix(Language lang) {
    return (lang == Language::PL) 
        ? "(Płótno pod tapetę z anime w 4K)" 
        : "(4K Anime Wallpaper Canvas)";
}

std::string LocaleManager::getDeWmSuffix(Language lang) {
    return (lang == Language::PL) 
        ? "(300h spędzone na animacjach zamiast snu)" 
        : "(300h spent ricing animations instead of sleeping)";
}

std::string LocaleManager::getThemeValue(Language lang) {
    return (lang == Language::PL) 
        ? "Catppuccin Mocha [Poziom ricing'u: 9001]" 
        : "Catppuccin Mocha [Rice Level: 9001]";
}

std::string LocaleManager::getTerminalSuffix(Language lang) {
    return (lang == Language::PL) 
        ? "(akcelerowany GPU bloat z przezroczystością 99%)" 
        : "(GPU accelerated bloat with 99% blur)";
}

std::string LocaleManager::getCpuSuffix(Language lang) {
    return (lang == Language::PL) 
        ? "@ 100% (ogrzewa pokój kompilując z AUR)" 
        : "@ 100% (heating bedroom while compiling AUR)";
}

std::string LocaleManager::getGpuSuffix(Language lang) {
    return (lang == Language::PL) 
        ? "(dedykowana do Discorda i przezroczystości w terminalu)" 
        : "(dedicated to Discord & terminal blur)";
}

std::string LocaleManager::getMemorySuffix(Language lang) {
    return (lang == Language::PL) 
        ? "(94% zjedzone przez Discorda i 50 kart Arch Wiki)" 
        : "(94% consumed by Discord & 50 Arch Wiki tabs)";
}

std::string LocaleManager::getDiskSuffix(Language lang) {
    return (lang == Language::PL) 
        ? "(90% to nieoczyszczony cache pacmana)" 
        : "(90% uncleaned pacman cache bloat)";
}

std::string LocaleManager::getGrassValue(Language lang) {
    return (lang == Language::PL) 
        ? "0.00 ms (Rekord życiowy: trawnik nie wspiera Waylanda)" 
        : "0.00 ms (Lifetime record: lawn doesn't support Wayland)";
}

std::string LocaleManager::getShowerValue(Language lang) {
    return (lang == Language::PL) 
        ? "Krytyczne 0 dni z rzędu (Higiena odrzucona jako bloat)" 
        : "Critical 0 days streak (Hygiene rejected as bloat)";
}

std::string LocaleManager::getGirlfriendValue(Language lang) {
    return (lang == Language::PL) 
        ? "Błąd segmentacji (zrzut pamięci) - Brak w repozytoriach" 
        : "Segmentation fault (core dumped) - 404 Not Found";
}

std::string LocaleManager::getHelpText(Language lang) {
    if (lang == Language::PL) {
        return 
            "\033[1;36mFATfetch\033[0m - Najbardziej przypakowany, spasiony i memiczny fetch pod Arch Linuxa (C++)\n\n"
            "\033[1mUŻYCIE:\033[0m\n"
            "  fatfetch [OPCJE]\n\n"
            "\033[1mOPCJE:\033[0m\n"
            "  -l, --logo <nazwa>    Wybierz ASCII logo: archguy (domyślne), fatarch, discordmod, minimal\n"
            "  -L, --lang <pl|en>    Wybierz język (polski / english)\n"
            "  -j, --joke            Wypisz tylko świeży, losowy żart/roast o użytkownikach Archa\n"
            "  -d, --diet            Wypisz oficjalną dietę użytkownika Archa (4500 kcal)\n"
            "  -r, --raw             Wyłącz kolory ANSI i formatowanie\n"
            "  -v, --version         Wyświetl wersję FATfetcha\n"
            "  -h, --help            Wyświetl tę pomoc\n\n"
            "\033[1;33mPamiętaj:\033[0m Prysznic to bloat. BTW, I use Arch.\n";
    } else {
        return 
            "\033[1;36mFATfetch\033[0m - The heaviest, most bloated meme fetch for Arch Linux (C++)\n\n"
            "\033[1mUSAGE:\033[0m\n"
            "  fatfetch [OPTIONS]\n\n"
            "\033[1mOPTIONS:\033[0m\n"
            "  -l, --logo <name>     Select ASCII logo: archguy (default), fatarch, discordmod, minimal\n"
            "  -L, --lang <pl|en>    Select language (english / polski)\n"
            "  -j, --joke            Print only a fresh random Arch Linux joke/roast\n"
            "  -d, --diet            Print the official Arch Linux Chad 4500 kcal diet plan\n"
            "  -r, --raw             Disable ANSI colors and formatting\n"
            "  -v, --version         Display FATfetch version\n"
            "  -h, --help            Display this help menu\n\n"
            "\033[1;33mRemember:\033[0m Showers are bloat. BTW, I use Arch.\n";
    }
}

std::string LocaleManager::getVersionText(Language lang) {
    if (lang == Language::PL) {
        return "\033[1;36mFATfetch\033[0m v4.2.0-bloated-edition (Arch Linux x86_64, C++20)\n"
               "Stworzone specjalnie dla elity r/unixporn i entuzjastów koszulek 6XL.\n";
    } else {
        return "\033[1;36mFATfetch\033[0m v4.2.0-bloated-edition (Arch Linux x86_64, C++20)\n"
               "Crafted specifically for the r/unixporn elite and 6XL t-shirt connoisseurs.\n";
    }
}

} // namespace FATfetch
