#pragma once
#include <string>
#include <vector>

namespace FATfetch {

enum class Language {
    PL,
    EN
};

struct FieldLabels {
    std::string os;
    std::string host;
    std::string kernel;
    std::string uptime;
    std::string packages;
    std::string shell;
    std::string resolution;
    std::string de_wm;
    std::string theme;
    std::string terminal;
    std::string cpu;
    std::string gpu;
    std::string memory;
    std::string disk;
    std::string grass;
    std::string shower;
    std::string girlfriend;
    std::string jokeBannerTitle;
};

class LocaleManager {
public:
    static Language detectSystemLanguage();
    static Language parseLanguage(const std::string& langStr);
    static std::string languageToString(Language lang);

    static FieldLabels getLabels(Language lang);
    static std::string getHelpText(Language lang);
    static std::string getVersionText(Language lang);
    static std::string getTitleSuffix(Language lang);

    // Dynamic roasts & suffixes
    static std::string getHostSuffix(Language lang);
    static std::string getKernelSuffix(Language lang);
    static std::string getUptimeSuffix(Language lang);
    static std::string getPackagesSuffix(Language lang, size_t count, size_t flatpaks);
    static std::string getShellSuffix(Language lang);
    static std::string getResolutionSuffix(Language lang);
    static std::string getDeWmSuffix(Language lang);
    static std::string getThemeValue(Language lang);
    static std::string getTerminalSuffix(Language lang);
    static std::string getCpuSuffix(Language lang);
    static std::string getGpuSuffix(Language lang);
    static std::string getMemorySuffix(Language lang);
    static std::string getDiskSuffix(Language lang);
    static std::string getGrassValue(Language lang);
    static std::string getShowerValue(Language lang);
    static std::string getGirlfriendValue(Language lang);
};

} // namespace FATfetch
