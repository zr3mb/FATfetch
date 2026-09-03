#include "discord_rpc.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string cmd = argv[1];
        if (cmd == "enable" || cmd == "--enable") {
            if (FATfetch::DiscordRPC::enableAutostart()) {
                std::cout << "\033[1;32m✔ Pomyślnie włączono autostart Discord Rich Presence (systemd & XDG)!\033[0m\n";
            }
            return 0;
        } else if (cmd == "disable" || cmd == "--disable") {
            if (FATfetch::DiscordRPC::disableAutostart()) {
                std::cout << "\033[1;33m✔ Wyłączono autostart Discord Rich Presence.\033[0m\n";
            }
            return 0;
        } else if (cmd == "status" || cmd == "--status") {
            FATfetch::DiscordRPC::printStatus();
            return 0;
        } else if (cmd == "daemon" || cmd == "--daemon" || cmd == "-d") {
            FATfetch::DiscordRPC::runDaemon(false);
            return 0;
        } else if (cmd == "stop" || cmd == "--stop") {
            FATfetch::DiscordRPC::stopDaemonBackground();
            std::cout << "\033[1;33m✔ Zatrzymano procesy demona Discord RPC w tle.\033[0m\n";
            return 0;
        } else if (cmd == "-h" || cmd == "--help") {
            std::cout << "UŻYCIE: fatrpc [enable|disable|status|stop|daemon]\n\n"
                      << "OPCJE:\n"
                      << "  enable     Włącz autostart demona Discord RPC przy starcie systemu\n"
                      << "  disable    Wyłącz autostart i zatrzymaj demona\n"
                      << "  status     Wyświetl status połączenia z Discordem i demona\n"
                      << "  stop       Zatrzymaj demona działającego w tle\n"
                      << "  (brak)     Uruchom Discord Rich Presence w trybie podglądu na żywo\n\n";
            return 0;
        }
    }

    // Default: run foreground daemon
    FATfetch::DiscordRPC::runDaemon(true);
    return 0;
}
