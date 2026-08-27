#include "../locales.hpp"
#include "../config.hpp"
#include "../tui_config.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

namespace fs = std::filesystem;
using FATfetch::Language;
using FATfetch::LocaleManager;
using FATfetch::AppConfig;
using FATfetch::ConfigManager;
using FATfetch::TuiConfigurator;

void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void resetCursor() {
    std::cout << "\033[H" << std::flush;
}

void hideCursor() {
    std::cout << "\033[?25l" << std::flush;
}

void showCursor() {
    std::cout << "\033[?25h" << std::flush;
}

void sleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int getTermWidth() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) {
        return w.ws_col;
    }
    return 80;
}

void printBanner(Language lang) {
    if (lang == Language::PL) {
        std::cout << "\033[1;36m"
                  << "╔════════════════════════════════════════════════════════════════════════════════╗\n"
                  << "║              FATfetch - OFICJALNY ARCH LINUX TUI INSTALATOR                    ║\n"
                  << "║       \"Bo Twój neofetch nie był wystarczająco spasiony ani potężny\"            ║\n"
                  << "╚════════════════════════════════════════════════════════════════════════════════╝\n"
                  << "\033[0m\n";
    } else {
        std::cout << "\033[1;36m"
                  << "╔════════════════════════════════════════════════════════════════════════════════╗\n"
                  << "║              FATfetch - OFFICIAL ARCH LINUX TUI INSTALLER                      ║\n"
                  << "║     \"Because your regular fetch wasn't bloated enough for an Arch Chad\"        ║\n"
                  << "╚════════════════════════════════════════════════════════════════════════════════╝\n"
                  << "\033[0m\n";
    }
}

void printArchGuyWelcome() {
    std::string C_RST = "\033[0m";
    std::string C_CYN = "\033[1;36m";
    std::string C_BLU = "\033[1;34m";
    std::string C_SKN = "\033[38;5;216m";
    std::string C_BLK = "\033[38;5;233m";
    std::string C_DGR = "\033[38;5;238m";

    std::cout <<
        C_DGR + "                  .---.             \n" +
        C_DGR + "                /       \\           \n" +
        C_SKN + "               |  " + C_BLK + "'-. .-" + C_SKN + " |          \n" +
        C_SKN + "               |  " + C_BLK + "o | o" + C_SKN + "  |          \n" +
        C_DGR + "               |   " + C_DGR + "###" + C_DGR + "   |          \n" +
        C_DGR + "                \\  " + C_DGR + "===" + C_DGR + "  /           \n" +
        C_BLK + "          .-------'     '-------.   \n" +
        C_BLK + "        /                         \\ \n" +
        C_BLK + "       /     " + C_CYN + "       /\\" + C_BLK + "               \\ \n" +
        C_BLK + "      /      " + C_CYN + "      /  \\" + C_BLK + "               \\\n" +
        C_BLK + "     /       " + C_CYN + "     / /\\ \\" + C_BLK + "              \\\n" +
        C_BLK + "    |        " + C_CYN + "    / /__\\ \\" + C_BLK + "               |\n" +
        C_BLK + "    |        " + C_CYN + "   /_/    \\_\\" + C_BLK + "              |\n" +
        C_BLK + "    |        " + C_CYN + "  archlinux" + C_BLK + "              |\n" +
        C_BLK + "    |                               |\n" +
        C_BLK + "     \\                             / \n" +
        C_SKN + "      '---.___________________..--'  \n" +
        C_SKN + "          (   ABSOLUTE UNIT   )      \n" +
        C_BLK + "           |       |       |         \n" +
        C_BLU + "          /___)         (___\\        \n" + C_RST;
}

// -----------------------------------------------------------------------------
//  🏃💨 ULTRA-DETAILED RUNNING & BELLY JIGGLE ANIMATION SYSTEM
// -----------------------------------------------------------------------------

struct RunningFrame {
    std::vector<std::string> lines;
};

std::vector<RunningFrame> getRunningFrames() {
    std::string C_RST = "\033[0m";
    std::string C_CYN = "\033[1;36m";
    std::string C_SKN = "\033[38;5;216m";
    std::string C_BLK = "\033[38;5;233m";
    std::string C_DGR = "\033[38;5;238m";
    std::string C_BLU = "\033[1;34m";
    std::string C_YEL = "\033[1;33m";
    std::string C_SWT = "\033[1;36m"; // sweat drops

    std::vector<RunningFrame> frames(4);

    // Frame 0: Right leg leap forward, belly bounces UP, sweat flying
    frames[0].lines = {
        C_DGR + "       .---.       " + C_SWT + "💦 " + C_YEL + "*PUFF!*" + C_RST,
        C_SKN + "     / 'o.o'\\     " + C_SWT + "💧" + C_RST,
        C_SKN + "     |  " + C_DGR + "###" + C_SKN + " |   " + C_DGR + "/" + C_RST,
        C_BLK + "   .--' === '--' " + C_DGR + "/" + C_RST,
        C_BLK + "  /              \\  " + C_CYN + "   /\\" + C_RST,
        C_BLK + " /   " + C_SKN + "(  ^   ^  )" + C_BLK + "  \\ " + C_CYN + "  /  \\" + C_RST,
        C_BLK + "|    " + C_SKN + "( BEBECH  )" + C_BLK + "   |" + C_CYN + " / /\\ \\" + C_RST,
        C_BLK + " \\   " + C_SKN + "(  JIGGLE )" + C_BLK + "  / " + C_CYN + "/ /__\\ \\" + C_RST,
        C_BLK + "  \\   " + C_SKN + "'-------'" + C_BLK + "  /  " + C_CYN + "archlinux" + C_RST,
        C_SKN + "   '---.______.-'  " + C_RST,
        C_BLK + "      /      \\     " + C_RST,
        C_BLU + "    _/        \\_   " + C_RST
    };

    // Frame 1: Ground stomp, belly jiggles DOWN heavily
    frames[1].lines = {
        C_DGR + "       .---.       " + C_SWT + "💦 " + C_YEL + "*HUUH!*" + C_RST,
        C_SKN + "     / -.-  \\     " + C_SWT + "💧" + C_RST,
        C_SKN + "     |  " + C_DGR + "###" + C_SKN + " |  " + C_DGR + "_" + C_RST,
        C_BLK + "   .--' === '--' " + C_DGR + "\\" + C_RST,
        C_BLK + "  /               \\ " + C_CYN + "  /\\" + C_RST,
        C_BLK + " /     " + C_SKN + "(  v  v  )" + C_BLK + "  \\" + C_CYN + " /  \\" + C_RST,
        C_BLK + "|      " + C_SKN + "( BEBECH )" + C_BLK + "   |" + C_CYN + "/ /\\ \\" + C_RST,
        C_BLK + " \\     " + C_SKN + "(  BOUNCE)" + C_BLK + "  // /__\\ \\" + C_RST,
        C_BLK + "  \\     " + C_SKN + "'------'" + C_BLK + "  // " + C_CYN + "archlinux" + C_RST,
        C_SKN + "   '---.______.-'  " + C_RST,
        C_BLK + "     / /     \\ \\   " + C_RST,
        C_BLU + "   _/_/       \\_   " + C_YEL + "*TUP!*" + C_RST
    };

    // Frame 2: Left leg leap forward, belly bounces UP to the left, sweat sprays
    frames[2].lines = {
        C_DGR + "       .---.       " + C_SWT + "💦 " + C_YEL + "*SAPAĆ!*" + C_RST,
        C_SKN + "     / >.<  \\     " + C_SWT + "💧" + C_RST,
        C_SKN + "     |  " + C_DGR + "###" + C_SKN + " |   " + C_DGR + "\\" + C_RST,
        C_BLK + "   .--' === '--' " + C_DGR + "\\" + C_RST,
        C_BLK + "  /              \\  " + C_CYN + "   /\\" + C_RST,
        C_BLK + " /   " + C_SKN + "(  ^   ^  )" + C_BLK + "  \\ " + C_CYN + "  /  \\" + C_RST,
        C_BLK + "|    " + C_SKN + "( BEBECH  )" + C_BLK + "   |" + C_CYN + " / /\\ \\" + C_RST,
        C_BLK + " \\   " + C_SKN + "(  WOBBLE )" + C_BLK + "  / " + C_CYN + "/ /__\\ \\" + C_RST,
        C_BLK + "  \\   " + C_SKN + "'-------'" + C_BLK + "  /  " + C_CYN + "archlinux" + C_RST,
        C_SKN + "   '---.______.-'  " + C_RST,
        C_BLK + "      /      \\     " + C_RST,
        C_BLU + "     _/        \\_  " + C_RST
    };

    // Frame 3: Heavy landing, seismic shockwave
    frames[3].lines = {
        C_DGR + "       .---.       " + C_SWT + "💦 " + C_YEL + "*DUDU!*" + C_RST,
        C_SKN + "     / O.o  \\     " + C_SWT + "💧" + C_RST,
        C_SKN + "     |  " + C_DGR + "###" + C_SKN + " |   " + C_DGR + "/" + C_RST,
        C_BLK + "   .--' === '--' " + C_DGR + "/" + C_RST,
        C_BLK + "  /              \\  " + C_CYN + "  /\\" + C_RST,
        C_BLK + " /     " + C_SKN + "(  v  v  )" + C_BLK + " \\ " + C_CYN + " /  \\" + C_RST,
        C_BLK + "|      " + C_SKN + "( BEBECH )" + C_BLK + "  |" + C_CYN + "/ /\\ \\" + C_RST,
        C_BLK + " \\     " + C_SKN + "(  JIGGLE)" + C_BLK + " / / /__\\ \\" + C_RST,
        C_BLK + "  \\     " + C_SKN + "'------'" + C_BLK + " /  " + C_CYN + "archlinux" + C_RST,
        C_SKN + "   '---.______.-'  " + C_RST,
        C_BLK + "     \\ \\     / /   " + C_RST,
        C_BLU + "      \\_     \\_    " + C_YEL + "*BUM!*" + C_RST
    };

    return frames;
}

void printExhaustedArchGuy(int pos) {
    std::string pad(pos, ' ');
    std::string C_RST = "\033[0m";
    std::string C_CYN = "\033[1;36m";
    std::string C_SKN = "\033[38;5;216m";
    std::string C_BLK = "\033[38;5;233m";
    std::string C_DGR = "\033[38;5;238m";
    std::string C_BLU = "\033[1;34m";
    std::string C_YEL = "\033[1;33m";
    std::string C_RED = "\033[1;31m";
    std::string C_SWT = "\033[1;36m";

    std::cout << pad << C_DGR + "           .---.           " + C_SWT + "💦 💦 " + C_YEL + "* HAA... HAA... *" + C_RST + "\n";
    std::cout << pad << C_SKN + "          / x.x \\   " + C_SWT + "💦     " + C_RED + "* UFFF... ZDYCHAM... *" + C_RST + "\n";
    std::cout << pad << C_SKN + "         |  ---  | /       " + C_YEL + "* JEZU CHRYSTE... *" + C_RST + "\n";
    std::cout << pad << C_DGR + "         |  " + C_DGR + "###" + C_DGR + "  |/        " + C_SWT + "💧 💧 💧" + C_RST + "\n";
    std::cout << pad << C_BLK + "      .---' === '--.       " + C_CYN + "   /\\" + C_RST + "\n";
    std::cout << pad << C_BLK + "     /              \\      " + C_CYN + "  /  \\" + C_RST + "\n";
    std::cout << pad << C_BLK + "    /   " + C_SKN + "(  WISZĄCY  )" + C_BLK + " \\     " + C_CYN + " / /\\ \\" + C_RST + "\n";
    std::cout << pad << C_BLK + "   |    " + C_SKN + "(  BEBECH   )" + C_BLK + "  |    " + C_CYN + "/ /__\\ \\" + C_RST + "\n";
    std::cout << pad << C_BLK + "   |    " + C_SKN + "( DO ZIEMI  )" + C_BLK + "  |    " + C_CYN + "archlinux" + C_RST + "\n";
    std::cout << pad << C_SKN + "    \\    '----------'   /   " + C_RST + "\n";
    std::cout << pad << C_SKN + "     '--.____________.-'    " + C_RST + "\n";
    std::cout << pad << C_BLK + "      /  /  |    \\  \\      " + C_YEL + "[ RĘCE NA KOLANACH ]" + C_RST + "\n";
    std::cout << pad << C_BLU + "     (O_O)  |     (O_O)     " + C_RST + "\n";
    std::cout << pad << C_BLU + "     / /    |      \\ \\      " + C_RST + "\n";
    std::cout << pad << C_BLU + "    (_/     |       \\_)     " + C_RST + "\n";
}

void runInstallationAnimation(Language lang) {
    hideCursor();
    clearScreen();

    auto frames = getRunningFrames();
    int termWidth = getTermWidth();
    if (termWidth < 70) termWidth = 70;

    int maxRunnerPos = termWidth - 42;
    if (maxRunnerPos < 10) maxRunnerPos = 10;
    if (maxRunnerPos > 40) maxRunnerPos = 40;

    struct StatusMsg {
        int percent;
        std::string pl;
        std::string en;
    };

    std::vector<StatusMsg> msgs = {
        {0,  "Start biegu! Bebech wpada w rezonans sejsmiczny...", "Start running! Belly enters seismic resonance..."},
        {15, "0.00ms dotkniętej trawy... Przyspieszanie masy...", "0.00ms grass touched... Accelerating 300kg mass..."},
        {30, "Kompilowanie C++20... Wylewanie 5 litrów potu...", "Compiling C++20... Pouring 5 liters of sweat..."},
        {50, "Bebech podskakuje z częstotliwością 140 Hz (Wayland)...", "Belly bouncing at 140 Hz (Wayland smooth)..."},
        {70, "Pobieranie zapasów Monster Energy z AUR na trasie...", "Grabbing Monster Energy cans from AUR stations..."},
        {85, "Widzę metę! Płuca odmawiają posłuszeństwa! UFF...", "Finish line in sight! Lungs at 100% capacity..."},
        {98, "Ostatni skok z rozpędu przez linię mety...", "Final massive leap across the finish line..."},
        {100, "META OSIĄGNIĘTA! INSTALACJA ZAKOŃCZONA!", "FINISH LINE REACHED! INSTALLATION COMPLETE!"}
    };

    int totalSteps = 45;
    for (int step = 0; step <= totalSteps; ++step) {
        resetCursor();

        int percent = (step * 100) / totalSteps;
        int runnerPos = (step * maxRunnerPos) / totalSteps;
        int frameIdx = step % 4;

        printBanner(lang);

        // Find status message
        std::string currentStatus = msgs[0].pl;
        for (const auto& m : msgs) {
            if (percent >= m.percent) {
                currentStatus = (lang == Language::PL) ? m.pl : m.en;
            }
        }

        std::cout << "\033[1;33m"
                  << ((lang == Language::PL) ? " [ ETAP 4/4: MARATON INSTALACYJNY ARCH CHADA ]\n\n" : " [ STEP 4/4: ARCH CHAD INSTALLATION MARATHON ]\n\n")
                  << "\033[0m";

        // Render the running guy frame at offset `runnerPos`
        std::string pad(runnerPos, ' ');
        const auto& curFrame = frames[frameIdx];

        int finishCol = maxRunnerPos + 32;
        int spaceAfterRunner = finishCol - runnerPos - 28;
        if (spaceAfterRunner < 1) spaceAfterRunner = 1;

        for (size_t lineIdx = 0; lineIdx < curFrame.lines.size(); ++lineIdx) {
            std::cout << pad << curFrame.lines[lineIdx];
            
            // Render Finish Line banner on the right side
            if (lineIdx == 0) {
                std::cout << std::string(spaceAfterRunner, ' ') << "\033[1;37;41m 🏁 META / FINISH 🏁 \033[0m";
            } else if (lineIdx < 11) {
                std::cout << std::string(spaceAfterRunner + 5, ' ') << "\033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m";
            }
            std::cout << "\n";
        }

        // Render detailed Progress Bar
        int barWidth = 40;
        int filled = (percent * barWidth) / 100;
        std::string barStr = "";
        for (int b = 0; b < barWidth; ++b) {
            if (b < filled) barStr += "█";
            else if (b == filled) barStr += "▒";
            else barStr += "░";
        }

        std::cout << "\n \033[1;36m[" << barStr << "] \033[1;32m" << percent << "%\033[0m\n";
        std::cout << " \033[1;37m" << currentStatus << "\033[0m\033[K\n";

        sleepMs(65);
    }

    // Finished: Exhausted guy at the finish line!
    clearScreen();
    printBanner(lang);

    std::cout << "\033[1;32m"
              << ((lang == Language::PL) ? " [ MARATON ZAKOŃCZONY - GRUBAS DOBIEGŁ DO METY! ]\n\n" : " [ MARATHON FINISHED - CHAD CROSSED THE FINISH LINE! ]\n\n")
              << "\033[0m";

    printExhaustedArchGuy(maxRunnerPos);

    std::cout << "\n\033[1;32m"
              << "╔════════════════════════════════════════════════════════════════════════════════╗\n"
              << "║                   🏁  INSTALLATION COMPLETE!  🏁                               ║\n"
              << "║              INSTALACJA FATfetch ZAKOŃCZONA SUKCESEM!                          ║\n"
              << "╚════════════════════════════════════════════════════════════════════════════════╝\n"
              << "\033[0m\n";

    showCursor();
}

void configureShellIntegration(const std::string& homeDir, const std::string& installedPath, bool autoStart, Language lang) {
    std::vector<std::string> rcFiles = {
        homeDir + "/.bashrc",
        homeDir + "/.zshrc",
        homeDir + "/.profile",
        homeDir + "/.bash_profile"
    };

    for (const auto& rc : rcFiles) {
        std::string content = "";
        if (fs::exists(rc)) {
            std::ifstream infile(rc);
            content = std::string((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
            infile.close();
        }

        std::ofstream outfile(rc, std::ios::app);
        if (content.find(".local/bin") == std::string::npos) {
            outfile << "\n# Added by FATfetch installer\nexport PATH=\"$HOME/.local/bin:$PATH\"\n";
        }
        if (content.find("alias fatfetch=") == std::string::npos && installedPath.find(".local/bin") != std::string::npos) {
            outfile << "alias fatfetch=\"" + installedPath + "\"\n";
        }
        if (autoStart && content.find("\nfatfetch\n") == std::string::npos && content.find("fatfetch") == std::string::npos) {
            outfile << "\n# FATfetch - Startup\nfatfetch\n";
        }
        outfile.close();
    }

    if (lang == Language::PL) {
        std::cout << "\033[1;32m✔ Zaktualizowano zmienną PATH i aliasy w plikach konfiguracyjnych powłoki (.bashrc, .zshrc).\033[0m\n";
    } else {
        std::cout << "\033[1;32m✔ Updated PATH and aliases in shell configuration files (.bashrc, .zshrc).\033[0m\n";
    }
}

int main(int argc, char* argv[]) {
    Language lang = LocaleManager::detectSystemLanguage();
    bool langPreselected = false;

    // Fast flag check
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-v" || arg == "--version") {
            std::cout << "fatfetch-installer v4.2.0\n";
            return 0;
        } else if (arg == "-h" || arg == "--help") {
            std::cout << "Usage: ./fatfetch-installer [--lang=pl|en]\n";
            return 0;
        } else if (arg == "-c" || arg == "--config") {
            TuiConfigurator::run();
            return 0;
        } else if ((arg == "-L" || arg == "--lang") && i + 1 < argc) {
            lang = LocaleManager::parseLanguage(argv[++i]);
            langPreselected = true;
        } else if (arg.rfind("--lang=", 0) == 0) {
            lang = LocaleManager::parseLanguage(arg.substr(7));
            langPreselected = true;
        }
    }

    clearScreen();
    printBanner(Language::PL);
    printArchGuyWelcome();

    // Language selection screen if not preselected
    if (!langPreselected) {
        std::cout << "\n\033[1;33m[ WYBIERZ JĘZYK / SELECT LANGUAGE ]\033[0m\n\n";
        std::cout << "  1) \033[1;37mPolski 🇵🇱\033[0m  (Domyślny)\n";
        std::cout << "  2) \033[1;37mEnglish 🇺🇸/🇬🇧\033[0m\n\n";
        std::cout << "Wybór / Choice [1-2] (1): ";

        std::string langChoice;
        std::getline(std::cin, langChoice);
        if (langChoice == "2" || langChoice == "en" || langChoice == "EN") {
            lang = Language::EN;
        } else {
            lang = Language::PL;
        }
    }

    // Welcome Screen
    clearScreen();
    printBanner(lang);
    printArchGuyWelcome();

    if (lang == Language::PL) {
        std::cout << "\n\033[1;33m[ WITAJ W INSTALATORZE FATfetch ]\033[0m\n";
        std::cout << "Czy spełniasz podstawowe wymagania:\n";
        std::cout << " • Używasz Arch Linuxa (lub chociaż mówisz wszystkim, że używasz)\n";
        std::cout << " • Nie brałeś prysznica od co najmniej 48 godzin\n";
        std::cout << " • Twój Neovim ma więcej niż 50 wtyczek\n";
        std::cout << " • Rozmiar koszulki: 4XL+ lub zakolanówki\n\n";
        std::cout << "\033[1;32mNaciśnij [ENTER], aby przejść do konfiguracji...\033[0m";
    } else {
        std::cout << "\n\033[1;33m[ WELCOME TO THE FATfetch INSTALLER ]\033[0m\n";
        std::cout << "Verify your qualifications before proceeding:\n";
        std::cout << " • You use Arch Linux (or tell everyone you do)\n";
        std::cout << " • Zero showers taken in the last 48 hours\n";
        std::cout << " • Neovim has over 50 custom Lua plugins\n";
        std::cout << " • T-Shirt size 4XL+ or striped programming socks\n\n";
        std::cout << "\033[1;32mPress [ENTER] to proceed to configuration...\033[0m";
    }

    std::string dummy;
    std::getline(std::cin, dummy);

    // KROK 1: Wybór Tożsamości / Identity Selection
    clearScreen();
    printBanner(lang);
    AppConfig initialConfig;
    initialConfig.lang = lang;

    if (lang == Language::PL) {
        std::cout << "\033[1;33m[ KROK 1/3: KIM CHCESZ BYĆ W FATfetch? ]\033[0m\n\n";
        std::cout << " 1) \033[1;36m🦣 Arch Chad Grubas\033[0m       (Klasyczny potężny gość w koszulce Arch Linux)\n";
        std::cout << " 2) \033[1;35m🌸 300kg Gruby Femboy\033[0m     (Gruby Femboy w zakolanówkach + Pastelowa paleta Femboy)\n";
        std::cout << " 3) \033[1;34m🏳️‍⚧️ Trans Pride Femboy\033[0m    (Femboy + pastelowa flaga Trans)\n";
        std::cout << " 4) \033[1;33m🦤 Discord Mod z piwnicy\033[0m  (Basement Dweller z brzuchem na wierzchu)\n";
        std::cout << " 5) \033[1;32m⚙️ Pełny Konfigurator TUI\033[0m (Otwórz graficzny edytor TUI z podglądem na żywo)\n\n";
        std::cout << "Wybierz swoją tożsamość [1-5] (domyślnie 1): ";
    } else {
        std::cout << "\033[1;33m[ STEP 1/3: WHO DO YOU WANT TO BE IN FATfetch? ]\033[0m\n\n";
        std::cout << " 1) \033[1;36m🦣 Arch Chad Big Guy\033[0m      (Classic absolute unit in Arch Linux T-shirt)\n";
        std::cout << " 2) \033[1;35m🌸 300kg Chonky Femboy\033[0m    (Thicc Femboy in programming socks + Femboy Palette)\n";
        std::cout << " 3) \033[1;34m🏳️‍⚧️ Trans Pride Femboy\033[0m    (Chonky Femboy + Trans Pride Palette)\n";
        std::cout << " 4) \033[1;33m🦤 Discord Basement Mod\033[0m   (Classic neckbeard moderator)\n";
        std::cout << " 5) \033[1;32m⚙️ Full TUI Configurator\033[0m  (Open interactive live preview configurator)\n\n";
        std::cout << "Select your identity [1-5] (default 1): ";
    }

    std::string personaChoice;
    std::getline(std::cin, personaChoice);

    if (personaChoice == "2") {
        initialConfig.logo = "fatfemboy";
        initialConfig.palette = "femboy";
    } else if (personaChoice == "3") {
        initialConfig.logo = "fatfemboy";
        initialConfig.palette = "trans";
    } else if (personaChoice == "4") {
        initialConfig.logo = "discordmod";
        initialConfig.palette = "default";
    } else if (personaChoice == "5") {
        TuiConfigurator::run();
        initialConfig = ConfigManager::loadConfig();
    } else {
        initialConfig.logo = "archguy";
        initialConfig.palette = "default";
    }

    // Save configured identity
    ConfigManager::saveConfig(initialConfig);

    // KROK 2: Install Path selection
    clearScreen();
    printBanner(lang);
    if (lang == Language::PL) {
        std::cout << "\033[1;33m[ KROK 2/3: WYBÓR LOKALIZACJI INSTALACJI ]\033[0m\n\n";
        std::cout << " 1) \033[1;36m/usr/local/bin/fatfetch\033[0m (Dla całego systemu - ZALECANE, natychmiast w PATH)\n";
        std::cout << " 2) \033[1;36m~/.local/bin/fatfetch\033[0m   (Dla użytkownika - bez uprawnień roota)\n";
        std::cout << " 3) \033[1;36mKompilacja lokalna\033[0m      (Pozostaw tylko plik binarny w bieżącym katalogu)\n\n";
        std::cout << "Wybierz opcję [1-3] (domyślnie 1): ";
    } else {
        std::cout << "\033[1;33m[ STEP 2/3: INSTALLATION TARGET ]\033[0m\n\n";
        std::cout << " 1) \033[1;36m/usr/local/bin/fatfetch\033[0m (System-wide - RECOMMENDED, globally in PATH)\n";
        std::cout << " 2) \033[1;36m~/.local/bin/fatfetch\033[0m   (User-only - NO root privileges required)\n";
        std::cout << " 3) \033[1;36mLocal build only\033[0m       (Keep compiled binary in current folder)\n\n";
        std::cout << "Select option [1-3] (default 1): ";
    }

    std::string choice;
    std::getline(std::cin, choice);
    if (choice.empty()) choice = "1";

    std::string installDir;
    std::string homeDir = getenv("HOME") ? getenv("HOME") : "/home/user";
    bool requiresSudo = false;

    if (choice == "1") {
        installDir = "/usr/local/bin";
        if (geteuid() != 0) requiresSudo = true;
    } else if (choice == "2") {
        installDir = homeDir + "/.local/bin";
    } else {
        installDir = "";
    }

    // KROK 3: Shell integration question
    clearScreen();
    printBanner(lang);
    if (lang == Language::PL) {
        std::cout << "\033[1;33m[ KROK 3/3: INTEGRACJA Z POWŁOKĄ ]\033[0m\n\n";
        std::cout << "Czy chcesz automatycznie dodać 'fatfetch' do pliku startowego Twojej powłoki\n"
                  << "(~/.bashrc lub ~/.zshrc), aby FATfetch witał Cię przy każdym otwarciu terminala?\n\n";
        std::cout << "Dodać do autostartu? [T/n]: ";
    } else {
        std::cout << "\033[1;33m[ STEP 3/3: SHELL STARTUP INTEGRATION ]\033[0m\n\n";
        std::cout << "Would you like to append 'fatfetch' to your shell startup file\n"
                  << "(~/.bashrc or ~/.zshrc) to remind you of your status every time you open a terminal?\n\n";
        std::cout << "Add to shell rc? [Y/n]: ";
    }

    std::string addShell;
    std::getline(std::cin, addShell);
    bool autoStart = (addShell.empty() || addShell == "t" || addShell == "T" || addShell == "y" || addShell == "Y" || addShell == "tak" || addShell == "yes");

    // Perform compilation if needed before starting animation
    if (!fs::exists("./fatfetch")) {
        system("make fatfetch >/dev/null 2>&1");
    }

    // 🏃 RUN THE EPIC BELLY JIGGLE & MARATHON INSTALLATION ANIMATION!
    runInstallationAnimation(lang);

    // File copy to destinations
    std::string userLocalBin = homeDir + "/.local/bin";
    std::error_code ec;
    fs::create_directories(userLocalBin, ec);
    fs::copy_file("./fatfetch", userLocalBin + "/fatfetch", fs::copy_options::overwrite_existing, ec);
    chmod((userLocalBin + "/fatfetch").c_str(), 0755);

    std::string targetPath = userLocalBin + "/fatfetch";
    if (installDir == "/usr/local/bin") {
        targetPath = installDir + "/fatfetch";
        if (requiresSudo) {
            std::string cmd = "sudo mkdir -p " + installDir + " && sudo cp -f ./fatfetch " + targetPath + " && sudo chmod 755 " + targetPath;
            system(cmd.c_str());
        } else {
            fs::create_directories(installDir, ec);
            fs::copy_file("./fatfetch", targetPath, fs::copy_options::overwrite_existing, ec);
            chmod(targetPath.c_str(), 0755);
        }
    }

    if (lang == Language::PL) {
        std::cout << "\n\033[1;32m✔ Zainstalowano najnowszą binarkę w: \033[1;37m" << targetPath << " oraz " << userLocalBin << "/fatfetch\033[0m\n";
    } else {
        std::cout << "\n\033[1;32m✔ Latest binary installed to: \033[1;37m" << targetPath << " and " << userLocalBin << "/fatfetch\033[0m\n";
    }

    configureShellIntegration(homeDir, targetPath, autoStart, lang);

    std::cout << "\n\033[1;32m════════════════════════════════════════════════════════════════════════════════\033[0m\n";
    if (lang == Language::PL) {
        std::cout << "\033[1;33m💡 WSKAZÓWKA:\033[0m Aby zmienić konfigurację w dowolnym momencie, wpisz:\n";
        std::cout << "   \033[1;36mfatfetch --config\033[0m\n";
        std::cout << "   W bieżącym oknie terminala wpisz: \033[1;36msource ~/.bashrc\033[0m (lub otwórz nowe okno terminala)!\n";
    } else {
        std::cout << "\033[1;33m💡 TIP:\033[0m To change settings anytime, run:\n";
        std::cout << "   \033[1;36mfatfetch --config\033[0m\n";
        std::cout << "   In current terminal run: \033[1;36msource ~/.bashrc\033[0m (or open a new terminal)!\n";
    }
    std::cout << "\033[1;32m════════════════════════════════════════════════════════════════════════════════\033[0m\n\n";

    if (lang == Language::PL) {
        std::cout << "Naciśnij [ENTER], aby odpalić pierwszy bieg FATfetch!\n";
    } else {
        std::cout << "Press [ENTER] to launch your first FATfetch run!\n";
    }
    std::getline(std::cin, dummy);

    clearScreen();
    std::string testCmd = targetPath;
    system(testCmd.c_str());

    return 0;
}
