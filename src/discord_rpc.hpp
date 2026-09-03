#pragma once
#include "config.hpp"
#include <string>

namespace FATfetch {

struct DiscordActivity {
    std::string state;
    std::string details;
    std::string largeImage = "archguy";
    std::string largeText = "FATfetch - Heaviest Linux Fetch";
    std::string smallImage = "arch";
    std::string smallText = "BTW, I use Arch";
    int64_t startTimestamp = 0;
};

class DiscordRPC {
public:
    static bool isAutostartEnabled();
    static bool enableAutostart();
    static bool disableAutostart();
    static bool startDaemonBackground();
    static bool stopDaemonBackground();
    static void runDaemon(bool foreground = false);
    static void printStatus();
};

} // namespace FATfetch
