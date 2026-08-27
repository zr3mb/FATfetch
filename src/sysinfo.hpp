#pragma once
#include "locales.hpp"
#include <string>
#include <vector>

namespace FATfetch {

struct SysInfo {
    Language lang = Language::PL;
    std::string username;
    std::string hostname;
    std::string userHostTitle;
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
    std::string archQuote;
    FieldLabels labels;
};

class SysInfoCollector {
public:
    static SysInfo collectInfo(Language lang = Language::PL);

private:
    static std::string getUsername();
    static std::string getHostname();
    static std::string getOS();
    static std::string getHostModel(Language lang);
    static std::string getKernel(Language lang);
    static std::string getUptime(Language lang);
    static std::string getPackages(Language lang);
    static std::string getShell(Language lang);
    static std::string getResolution(Language lang);
    static std::string getDEorWM(Language lang);
    static std::string getTheme(Language lang);
    static std::string getTerminal(Language lang);
    static std::string getCPU(Language lang);
    static std::string getGPU(Language lang);
    static std::string getMemory(Language lang);
    static std::string getDisk(Language lang);
};

} // namespace FATfetch
