#include "sysinfo.hpp"
#include "jokes.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <pwd.h>
#include <iomanip>

namespace fs = std::filesystem;

namespace FATfetch {

static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n\"");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n\"");
    return str.substr(first, (last - first + 1));
}

static std::string readFileFirstLine(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return "";
    std::string line;
    if (std::getline(file, line)) {
        return trim(line);
    }
    return "";
}

std::string SysInfoCollector::getUsername() {
    const char* user = std::getenv("USER");
    if (user && *user) return std::string(user);
    struct passwd* pw = getpwuid(geteuid());
    if (pw && pw->pw_name) return std::string(pw->pw_name);
    return "archlord";
}

std::string SysInfoCollector::getHostname() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        return std::string(hostname);
    }
    return "arch-battlestation";
}

std::string SysInfoCollector::getOS() {
    std::string prettyName;
    std::ifstream osFile("/etc/os-release");
    if (osFile.is_open()) {
        std::string line;
        while (std::getline(osFile, line)) {
            if (line.rfind("PRETTY_NAME=", 0) == 0) {
                prettyName = trim(line.substr(12));
                break;
            }
        }
    }
    if (prettyName.empty()) {
        prettyName = "Arch Linux";
    }

    struct utsname uts;
    std::string arch = "x86_64";
    if (uname(&uts) == 0) {
        arch = uts.machine;
    }

    return prettyName + " " + arch + " (btw i use arch)";
}

std::string SysInfoCollector::getHostModel(Language lang) {
    std::string product = readFileFirstLine("/sys/class/dmi/id/product_name");
    std::string version = readFileFirstLine("/sys/class/dmi/id/product_version");
    std::string suffix = " " + LocaleManager::getHostSuffix(lang);

    if (!product.empty() && product != "System Product Name" && product != "None") {
        std::string res = product;
        if (!version.empty() && version != "None") res += " (" + version + ")";
        return res + suffix;
    }

    std::string board = readFileFirstLine("/sys/class/dmi/id/board_name");
    if (!board.empty()) {
        return board + suffix;
    }

    return (lang == Language::PL) 
        ? "Własna stacja bojowa (Kable wszędzie)" + suffix 
        : "Custom DIY Battlestation (Cables everywhere)" + suffix;
}

std::string SysInfoCollector::getKernel(Language lang) {
    struct utsname uts;
    if (uname(&uts) == 0) {
        return std::string(uts.release) + " " + LocaleManager::getKernelSuffix(lang);
    }
    return "Linux 6.9.420-arch-bloat";
}

std::string SysInfoCollector::getUptime(Language lang) {
    std::ifstream uptimeFile("/proc/uptime");
    if (!uptimeFile.is_open()) return "420 hours (no sleep)";
    double totalSeconds = 0;
    uptimeFile >> totalSeconds;

    long seconds = static_cast<long>(totalSeconds);
    long days = seconds / 86400;
    long hours = (seconds % 86400) / 3600;
    long mins = (seconds % 3600) / 60;

    std::ostringstream ss;
    if (days > 0) ss << days << "d ";
    if (hours > 0 || days > 0) ss << hours << "h ";
    ss << mins << "m " << LocaleManager::getUptimeSuffix(lang);
    return ss.str();
}

std::string SysInfoCollector::getPackages(Language lang) {
    size_t pacmanCount = 0;
    std::error_code ec;
    if (fs::exists("/var/lib/pacman/local", ec)) {
        for (const auto& entry : fs::directory_iterator("/var/lib/pacman/local", ec)) {
            if (entry.is_directory()) {
                pacmanCount++;
            }
        }
    }

    size_t flatpaks = 0;
    if (fs::exists("/var/lib/flatpak/app", ec)) {
        for (const auto& entry : fs::directory_iterator("/var/lib/flatpak/app", ec)) {
            if (entry.is_directory()) flatpaks++;
        }
    }

    if (pacmanCount > 0) {
        return LocaleManager::getPackagesSuffix(lang, pacmanCount, flatpaks);
    }

    return (lang == Language::PL) ? "1337 (spasione pakiety z AUR)" : "1337 (all bloated AUR packages)";
}

std::string SysInfoCollector::getShell(Language lang) {
    const char* shellEnv = std::getenv("SHELL");
    std::string shellName = shellEnv ? shellEnv : "/bin/bash";
    size_t slash = shellName.find_last_of('/');
    if (slash != std::string::npos) {
        shellName = shellName.substr(slash + 1);
    }
    return shellName + " " + LocaleManager::getShellSuffix(lang);
}

std::string SysInfoCollector::getResolution(Language lang) {
    std::error_code ec;
    std::string drmPath = "/sys/class/drm";
    if (fs::exists(drmPath, ec)) {
        for (const auto& entry : fs::directory_iterator(drmPath, ec)) {
            std::string modeFile = entry.path().string() + "/modes";
            if (fs::exists(modeFile, ec)) {
                std::string mode = readFileFirstLine(modeFile);
                if (!mode.empty()) {
                    return mode + " @ 144Hz " + LocaleManager::getResolutionSuffix(lang);
                }
            }
        }
    }
    return "1920x1080 " + LocaleManager::getResolutionSuffix(lang);
}

std::string SysInfoCollector::getDEorWM(Language lang) {
    const char* xdgDesktop = std::getenv("XDG_CURRENT_DESKTOP");
    const char* desktopSession = std::getenv("DESKTOP_SESSION");
    const char* waylandDisplay = std::getenv("WAYLAND_DISPLAY");

    std::string wm = "";
    if (xdgDesktop && *xdgDesktop) wm = xdgDesktop;
    else if (desktopSession && *desktopSession) wm = desktopSession;

    if (wm.empty()) {
        std::error_code ec;
        bool hasHyprland = fs::exists("/tmp/hypr", ec) || (waylandDisplay && std::string(waylandDisplay).find("wayland") != std::string::npos);
        if (hasHyprland) wm = "Hyprland";
        else wm = "i3-gaps";
    }

    return wm + " " + LocaleManager::getDeWmSuffix(lang);
}

std::string SysInfoCollector::getTheme(Language lang) {
    return LocaleManager::getThemeValue(lang);
}

std::string SysInfoCollector::getTerminal(Language lang) {
    const char* termProgram = std::getenv("TERM_PROGRAM");
    std::string termName;
    if (termProgram && *termProgram) {
        termName = termProgram;
    } else {
        const char* term = std::getenv("TERM");
        termName = (term && *term) ? term : "kitty";
    }
    return termName + " " + LocaleManager::getTerminalSuffix(lang);
}

std::string SysInfoCollector::getCPU(Language lang) {
    std::ifstream cpuFile("/proc/cpuinfo");
    std::string model = "";
    int cores = 0;
    if (cpuFile.is_open()) {
        std::string line;
        while (std::getline(cpuFile, line)) {
            if (line.rfind("model name", 0) == 0) {
                size_t colon = line.find(':');
                if (colon != std::string::npos && model.empty()) {
                    model = trim(line.substr(colon + 1));
                }
            }
            if (line.rfind("processor", 0) == 0) {
                cores++;
            }
        }
    }

    if (model.empty()) model = "AMD Ryzen / Intel Core";
    std::ostringstream ss;
    ss << model;
    if (cores > 0) ss << " (" << cores << ") ";
    ss << LocaleManager::getCpuSuffix(lang);
    return ss.str();
}

std::string SysInfoCollector::getGPU(Language lang) {
    FILE* fp = popen("lspci 2>/dev/null | grep -i 'vga\\|3d\\|display' | head -n 1", "r");
    if (fp) {
        char buffer[512];
        if (fgets(buffer, sizeof(buffer), fp)) {
            pclose(fp);
            std::string line(buffer);
            size_t colon = line.find(':');
            if (colon != std::string::npos && colon + 1 < line.length()) {
                size_t secondColon = line.find(':', colon + 1);
                std::string gpuName = (secondColon != std::string::npos) ? line.substr(secondColon + 1) : line.substr(colon + 1);
                gpuName = trim(gpuName);
                if (!gpuName.empty()) {
                    return gpuName + " " + LocaleManager::getGpuSuffix(lang);
                }
            }
        } else {
            pclose(fp);
        }
    }
    return "AMD Radeon / NVIDIA GeForce " + LocaleManager::getGpuSuffix(lang);
}

std::string SysInfoCollector::getMemory(Language lang) {
    std::ifstream memFile("/proc/meminfo");
    unsigned long long totalKB = 0;
    unsigned long long availKB = 0;

    if (memFile.is_open()) {
        std::string line;
        while (std::getline(memFile, line)) {
            if (line.rfind("MemTotal:", 0) == 0) {
                std::istringstream iss(line.substr(9));
                iss >> totalKB;
            } else if (line.rfind("MemAvailable:", 0) == 0) {
                std::istringstream iss(line.substr(13));
                iss >> availKB;
            }
        }
    }

    if (totalKB > 0) {
        unsigned long long usedKB = (totalKB > availKB) ? (totalKB - availKB) : 0;
        double usedGiB = usedKB / (1024.0 * 1024.0);
        double totalGiB = totalKB / (1024.0 * 1024.0);

        std::ostringstream ss;
        ss << std::fixed << std::setprecision(1) << usedGiB << " GiB / " << totalGiB << " GiB " << LocaleManager::getMemorySuffix(lang);
        return ss.str();
    }

    return "31.4 GiB / 32.0 GiB " + LocaleManager::getMemorySuffix(lang);
}

std::string SysInfoCollector::getDisk(Language lang) {
    struct statvfs stat;
    if (statvfs("/", &stat) == 0) {
        unsigned long long totalBytes = stat.f_blocks * stat.f_frsize;
        unsigned long long freeBytes = stat.f_bavail * stat.f_frsize;
        unsigned long long usedBytes = totalBytes - freeBytes;

        double usedGiB = usedBytes / (1024.0 * 1024.0 * 1024.0);
        double totalGiB = totalBytes / (1024.0 * 1024.0 * 1024.0);

        std::ostringstream ss;
        ss << std::fixed << std::setprecision(1) << usedGiB << " GiB / " << totalGiB << " GiB " << LocaleManager::getDiskSuffix(lang);
        return ss.str();
    }
    return "890 GiB / 1000 GiB " + LocaleManager::getDiskSuffix(lang);
}

SysInfo SysInfoCollector::collectInfo(Language lang) {
    SysInfo info;
    info.lang = lang;
    info.username = getUsername();
    info.hostname = getHostname();
    info.userHostTitle = info.username + "@" + info.hostname + " " + LocaleManager::getTitleSuffix(lang);
    info.os = getOS();
    info.host = getHostModel(lang);
    info.kernel = getKernel(lang);
    info.uptime = getUptime(lang);
    info.packages = getPackages(lang);
    info.shell = getShell(lang);
    info.resolution = getResolution(lang);
    info.de_wm = getDEorWM(lang);
    info.theme = getTheme(lang);
    info.terminal = getTerminal(lang);
    info.cpu = getCPU(lang);
    info.gpu = getGPU(lang);
    info.memory = getMemory(lang);
    info.disk = getDisk(lang);
    info.grass = LocaleManager::getGrassValue(lang);
    info.shower = LocaleManager::getShowerValue(lang);
    info.girlfriend = LocaleManager::getGirlfriendValue(lang);
    info.archQuote = JokeGenerator::getRandomJoke(lang);
    info.labels = LocaleManager::getLabels(lang);
    return info;
}

} // namespace FATfetch
