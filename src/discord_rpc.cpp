#include "discord_rpc.hpp"
#include "sysinfo.hpp"
#include "config.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <cstring>
#include <filesystem>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

namespace fs = std::filesystem;

namespace FATfetch {

static const std::string DISCORD_CLIENT_ID = "1212879505872162816"; // FATfetch Discord App ID
static volatile bool g_running = true;

static void rpcSignalHandler(int) {
    g_running = false;
}

static std::string escapeJson(const std::string& str) {
    std::string out;
    for (char c : str) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\b') out += "\\b";
        else if (c == '\f') out += "\\f";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else out += c;
    }
    return out;
}

class DiscordIpcConnection {
private:
    int socketFd = -1;

public:
    DiscordIpcConnection() = default;
    ~DiscordIpcConnection() {
        disconnect();
    }

    bool connect() {
        disconnect();

        std::vector<std::string> searchPaths;
        const char* xdgRuntime = std::getenv("XDG_RUNTIME_DIR");
        if (xdgRuntime && *xdgRuntime) {
            for (int i = 0; i < 10; ++i) {
                searchPaths.push_back(std::string(xdgRuntime) + "/discord-ipc-" + std::to_string(i));
                searchPaths.push_back(std::string(xdgRuntime) + "/app/com.discordapp.Discord/discord-ipc-" + std::to_string(i));
            }
        }
        for (int i = 0; i < 10; ++i) {
            searchPaths.push_back("/tmp/discord-ipc-" + std::to_string(i));
        }

        for (const auto& path : searchPaths) {
            if (!fs::exists(path)) continue;

            int fd = socket(AF_UNIX, SOCK_STREAM, 0);
            if (fd < 0) continue;

            struct sockaddr_un addr;
            std::memset(&addr, 0, sizeof(addr));
            addr.sun_family = AF_UNIX;
            std::strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);

            if (::connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
                socketFd = fd;
                // Perform handshake
                if (sendHandshake()) {
                    return true;
                }
                ::close(fd);
                socketFd = -1;
            } else {
                ::close(fd);
            }
        }

        return false;
    }

    void disconnect() {
        if (socketFd >= 0) {
            ::close(socketFd);
            socketFd = -1;
        }
    }

    bool isConnected() const {
        return socketFd >= 0;
    }

    bool sendPacket(uint32_t opcode, const std::string& jsonPayload) {
        if (socketFd < 0) return false;

        uint32_t len = static_cast<uint32_t>(jsonPayload.length());
        std::vector<uint8_t> buffer(8 + len);

        // Opcode (little endian)
        buffer[0] = static_cast<uint8_t>(opcode & 0xFF);
        buffer[1] = static_cast<uint8_t>((opcode >> 8) & 0xFF);
        buffer[2] = static_cast<uint8_t>((opcode >> 16) & 0xFF);
        buffer[3] = static_cast<uint8_t>((opcode >> 24) & 0xFF);

        // Length (little endian)
        buffer[4] = static_cast<uint8_t>(len & 0xFF);
        buffer[5] = static_cast<uint8_t>((len >> 8) & 0xFF);
        buffer[6] = static_cast<uint8_t>((len >> 16) & 0xFF);
        buffer[7] = static_cast<uint8_t>((len >> 24) & 0xFF);

        std::memcpy(buffer.data() + 8, jsonPayload.data(), len);

        ssize_t written = ::write(socketFd, buffer.data(), buffer.size());
        return written == static_cast<ssize_t>(buffer.size());
    }

    bool sendHandshake() {
        std::string payload = "{\"v\":1,\"client_id\":\"" + DISCORD_CLIENT_ID + "\"}";
        if (!sendPacket(0, payload)) return false;

        // Read response header
        uint8_t header[8];
        ssize_t n = ::read(socketFd, header, 8);
        if (n < 8) return false;

        uint32_t respLen = header[4] | (header[5] << 8) | (header[6] << 16) | (header[7] << 24);
        if (respLen > 0) {
            std::vector<char> respBody(respLen);
            ::read(socketFd, respBody.data(), respLen);
        }
        return true;
    }

    bool setActivity(const DiscordActivity& act) {
        std::ostringstream ss;
        ss << "{"
           << "\"cmd\":\"SET_ACTIVITY\","
           << "\"args\":{"
           << "\"pid\":" << getpid() << ","
           << "\"activity\":{"
           << "\"details\":\"" << escapeJson(act.details) << "\","
           << "\"state\":\"" << escapeJson(act.state) << "\","
           << "\"timestamps\":{\"start\":" << act.startTimestamp << "},"
           << "\"assets\":{"
           << "\"large_image\":\"" << escapeJson(act.largeImage) << "\","
           << "\"large_text\":\"" << escapeJson(act.largeText) << "\","
           << "\"small_image\":\"" << escapeJson(act.smallImage) << "\","
           << "\"small_text\":\"" << escapeJson(act.smallText) << "\""
           << "},"
           << "\"buttons\":["
           << "{\"label\":\"GitHub Repo\",\"url\":\"https://github.com/zr3mb/FATfetch\"},"
           << "{\"label\":\"Arch Linux Wiki\",\"url\":\"https://wiki.archlinux.org\"}"
           << "]"
           << "}"
           << "},"
           << "\"nonce\":\"1\""
           << "}";

        return sendPacket(1, ss.str());
    }
};

static std::string getSystemdServicePath() {
    const char* home = std::getenv("HOME");
    std::string base = home ? home : "/home/user";
    return base + "/.config/systemd/user/fatfetch-rpc.service";
}

static std::string getDesktopAutostartPath() {
    const char* home = std::getenv("HOME");
    std::string base = home ? home : "/home/user";
    return base + "/.config/autostart/fatfetch-rpc.desktop";
}

static std::string getExecutablePath() {
    const char* home = std::getenv("HOME");
    std::string userBin = home ? (std::string(home) + "/.local/bin/fatfetch") : "/usr/local/bin/fatfetch";
    if (fs::exists(userBin)) return userBin;
    if (fs::exists("/usr/local/bin/fatfetch")) return "/usr/local/bin/fatfetch";
    return "fatfetch";
}

bool DiscordRPC::isAutostartEnabled() {
    std::string servicePath = getSystemdServicePath();
    std::string desktopPath = getDesktopAutostartPath();
    return fs::exists(servicePath) || fs::exists(desktopPath);
}

bool DiscordRPC::enableAutostart() {
    std::string exe = getExecutablePath();
    std::string servicePath = getSystemdServicePath();
    std::string desktopPath = getDesktopAutostartPath();

    std::error_code ec;
    fs::create_directories(fs::path(servicePath).parent_path(), ec);
    fs::create_directories(fs::path(desktopPath).parent_path(), ec);

    // 1. Systemd User Service
    {
        std::ofstream sfile(servicePath);
        if (sfile.is_open()) {
            sfile << "[Unit]\n"
                  << "Description=FATfetch Discord Rich Presence Daemon\n"
                  << "After=network.target\n\n"
                  << "[Service]\n"
                  << "Type=simple\n"
                  << "ExecStart=" << exe << " --rpc-daemon\n"
                  << "Restart=on-failure\n"
                  << "RestartSec=15\n\n"
                  << "[Install]\n"
                  << "WantedBy=default.target\n";
            sfile.close();
        }
    }

    // 2. XDG Autostart Desktop file
    {
        std::ofstream dfile(desktopPath);
        if (dfile.is_open()) {
            dfile << "[Desktop Entry]\n"
                  << "Type=Application\n"
                  << "Name=FATfetch Discord RPC\n"
                  << "Comment=Discord Rich Presence for FATfetch Arch Linux\n"
                  << "Exec=" << exe << " --rpc-daemon\n"
                  << "Hidden=false\n"
                  << "NoDisplay=true\n"
                  << "X-GNOME-Autostart-enabled=true\n";
            dfile.close();
        }
    }

    // Attempt to enable systemd service if systemd is active
    system("systemctl --user daemon-reload >/dev/null 2>&1 || true");
    system("systemctl --user enable --now fatfetch-rpc.service >/dev/null 2>&1 || true");

    return true;
}

bool DiscordRPC::disableAutostart() {
    std::string servicePath = getSystemdServicePath();
    std::string desktopPath = getDesktopAutostartPath();

    system("systemctl --user stop fatfetch-rpc.service >/dev/null 2>&1 || true");
    system("systemctl --user disable fatfetch-rpc.service >/dev/null 2>&1 || true");

    std::error_code ec;
    if (fs::exists(servicePath)) fs::remove(servicePath, ec);
    if (fs::exists(desktopPath)) fs::remove(desktopPath, ec);

    system("systemctl --user daemon-reload >/dev/null 2>&1 || true");
    stopDaemonBackground();

    return true;
}

bool DiscordRPC::startDaemonBackground() {
    std::string exe = getExecutablePath();
    std::string cmd = "nohup " + exe + " --rpc-daemon >/dev/null 2>&1 &";
    int res = system(cmd.c_str());
    return res == 0;
}

bool DiscordRPC::stopDaemonBackground() {
    int res = system("pkill -f 'fatfetch --rpc-daemon' >/dev/null 2>&1");
    return res == 0;
}

void DiscordRPC::printStatus() {
    std::cout << "\n\033[1;36m[ FATfetch - DISCORD RICH PRESENCE (RPC) STATUS ]\033[0m\n\n";

    bool autostart = isAutostartEnabled();
    std::cout << "  • Autostart (systemd/XDG): " << (autostart ? "\033[1;32m[WŁĄCZONY / ENABLED]\033[0m" : "\033[1;31m[WYŁĄCZONY / DISABLED]\033[0m") << "\n";

    int runningCheck = system("pgrep -f 'fatfetch --rpc-daemon' >/dev/null 2>&1");
    bool running = (runningCheck == 0);
    std::cout << "  • Status demona:           " << (running ? "\033[1;32m[URUCHOMIONY W TLE / RUNNING]\033[0m" : "\033[1;33m[ZATRZYMANY / STOPPED]\033[0m") << "\n";

    DiscordIpcConnection testConn;
    bool discordOpen = testConn.connect();
    std::cout << "  • Połączenie z Discordem:  " << (discordOpen ? "\033[1;32m[POŁĄCZONO / CONNECTED]\033[0m" : "\033[1;31m[BRAK DISCORDA / NOT FOUND]\033[0m") << "\n\n";

    std::cout << "\033[1;33mKOMENDY STEROWANIA RPC:\033[0m\n"
              << "  fatfetch --rpc-enable     Włącz autostart demona Discord RPC\n"
              << "  fatfetch --rpc-disable    Wyłącz autostart i zatrzymaj demona\n"
              << "  fatfetch --rpc            Uruchom RPC w bieżącym terminalu (podgląd na żywo)\n"
              << "  fatfetch --rpc-status     Wyświetl ten status\n\n";
}

void DiscordRPC::runDaemon(bool foreground) {
    signal(SIGINT, rpcSignalHandler);
    signal(SIGTERM, rpcSignalHandler);

    if (foreground) {
        std::cout << "\033[1;36m[FATfetch RPC]\033[0m Uruchamianie Discord Rich Presence (Naciśnij Ctrl+C, aby zatrzymać)...\n";
    }

    DiscordIpcConnection conn;
    int64_t startTime = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    while (g_running) {
        if (!conn.isConnected()) {
            if (conn.connect()) {
                if (foreground) {
                    std::cout << "\033[1;32m[FATfetch RPC] Pomyślnie połączono z Discordem!\033[0m\n";
                }
            } else {
                // Wait before next reconnect attempt
                for (int i = 0; i < 10 && g_running; ++i) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
                continue;
            }
        }

        AppConfig cfg = ConfigManager::loadConfig();
        SysInfo info = SysInfoCollector::collectInfo(cfg.lang);

        DiscordActivity act;
        act.startTimestamp = startTime;

        if (cfg.logo == "fatfemboy") {
            act.details = "🌸 300kg UwU Chonky Femboy";
            act.state = "Socks: Thigh High | Monster: Empty";
            act.largeImage = "femboy";
            act.largeText = "FATfetch - 300kg Femboy Edition";
            act.smallImage = "arch";
            act.smallText = "Arch Linux UwU";
        } else if (cfg.logo == "discordmod") {
            act.details = "🦤 Discord Basement Moderator";
            act.state = "Banning users & ignoring showers";
            act.largeImage = "discordmod";
            act.largeText = "FATfetch - Moderator Edition";
            act.smallImage = "arch";
            act.smallText = "Arch Linux";
        } else { // default archguy
            act.details = "300kg Arch Linux Chad (btw i use arch)";
            act.state = "Shower: 0 days | Grass: 0.00ms";
            act.largeImage = "archguy";
            act.largeText = "FATfetch v4.2.0 - 6XL T-Shirt Chad";
            act.smallImage = "arch";
            act.smallText = "Arch Linux x86_64";
        }

        if (!conn.setActivity(act)) {
            if (foreground) {
                std::cout << "\033[1;31m[FATfetch RPC] Utracono połączenie z Discordem. Próba ponownego połączenia...\033[0m\n";
            }
            conn.disconnect();
        } else if (foreground) {
            std::cout << "\033[1;32m✔ [FATfetch RPC] Zaktualizowano status:\033[0m " << act.details << " | " << act.state << "\n";
        }

        // Sleep 15 seconds before next update
        for (int i = 0; i < 30 && g_running; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    conn.disconnect();
    if (foreground) {
        std::cout << "\n\033[1;33m[FATfetch RPC] Zatrzymano Discord Rich Presence.\033[0m\n";
    }
}

} // namespace FATfetch
