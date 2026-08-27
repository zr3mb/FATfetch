#include "../locales.hpp"
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

namespace fs = std::filesystem;
using FATfetch::Language;
using FATfetch::LocaleManager;

void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void sleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void printArchGuy() {
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

void configureShellIntegration(const std::string& homeDir, const std::string& installedPath, bool autoStart, Language lang) {
    std::string binDir = homeDir + "/.local/bin";
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
    printArchGuy();

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
    printArchGuy();

    if (lang == Language::PL) {
        std::cout << "\n\033[1;33m[ WITAJ W INSTALATORZE FATfetch ]\033[0m\n";
        std::cout << "Czy spełniasz podstawowe wymagania:\n";
        std::cout << " • Używasz Arch Linuxa (lub chociaż mówisz wszystkim, że używasz)\n";
        std::cout << " • Nie brałeś prysznica od co najmniej 48 godzin\n";
        std::cout << " • Twój Neovim ma więcej niż 50 wtyczek\n";
        std::cout << " • Rozmiar koszulki: 4XL+\n\n";
        std::cout << "\033[1;32mNaciśnij [ENTER], aby rozpocząć proces instalacji...\033[0m";
    } else {
        std::cout << "\n\033[1;33m[ WELCOME TO THE FATfetch INSTALLER ]\033[0m\n";
        std::cout << "Verify your qualifications before proceeding:\n";
        std::cout << " • You use Arch Linux (or tell everyone you do)\n";
        std::cout << " • Zero showers taken in the last 48 hours\n";
        std::cout << " • Neovim has over 50 custom Lua plugins\n";
        std::cout << " • T-Shirt size: 4XL+\n\n";
        std::cout << "\033[1;32mPress [ENTER] to begin the installation wizard...\033[0m";
    }

    std::string dummy;
    std::getline(std::cin, dummy);

    // Step 1: Install Path selection
    clearScreen();
    printBanner(lang);
    if (lang == Language::PL) {
        std::cout << "\033[1;33m[ KROK 1/3: WYBÓR LOKALIZACJI INSTALACJI ]\033[0m\n\n";
        std::cout << " 1) \033[1;36m/usr/local/bin/fatfetch\033[0m (Dla całego systemu - ZALECANE, natychmiast w PATH)\n";
        std::cout << " 2) \033[1;36m~/.local/bin/fatfetch\033[0m   (Dla użytkownika - bez uprawnień roota)\n";
        std::cout << " 3) \033[1;36mKompilacja lokalna\033[0m      (Pozostaw tylko plik binarny w bieżącym katalogu)\n\n";
        std::cout << "Wybierz opcję [1-3] (domyślnie 1): ";
    } else {
        std::cout << "\033[1;33m[ STEP 1/3: INSTALLATION TARGET ]\033[0m\n\n";
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

    // Step 2: Shell integration question
    clearScreen();
    printBanner(lang);
    if (lang == Language::PL) {
        std::cout << "\033[1;33m[ KROK 2/3: INTEGRACJA Z POWŁOKĄ ]\033[0m\n\n";
        std::cout << "Czy chcesz automatycznie dodać 'fatfetch' do pliku startowego Twojej powłoki\n"
                  << "(~/.bashrc lub ~/.zshrc), aby FATfetch witał Cię przy każdym otwarciu terminala?\n\n";
        std::cout << "Dodać do autostartu? [T/n]: ";
    } else {
        std::cout << "\033[1;33m[ STEP 2/3: SHELL STARTUP INTEGRATION ]\033[0m\n\n";
        std::cout << "Would you like to append 'fatfetch' to your shell startup file\n"
                  << "(~/.bashrc or ~/.zshrc) to remind you of your Arch Chad status every time you open a terminal?\n\n";
        std::cout << "Add to shell rc? [Y/n]: ";
    }

    std::string addShell;
    std::getline(std::cin, addShell);
    bool autoStart = (addShell.empty() || addShell == "t" || addShell == "T" || addShell == "y" || addShell == "Y" || addShell == "tak" || addShell == "yes");

    // Step 3: Compilation / Verification and live progress
    clearScreen();
    printBanner(lang);
    if (lang == Language::PL) {
        std::cout << "\033[1;33m[ KROK 3/3: INSTALACJA I INICJALIZACJA ]\033[0m\n\n";
    } else {
        std::cout << "\033[1;33m[ STEP 3/3: INSTALLATION & INITIALIZATION ]\033[0m\n\n";
    }

    struct Step {
        int percent;
        std::string msgPL;
        std::string msgEN;
    };

    std::vector<Step> steps = {
        {10, "Sprawdzanie poziomu bloatu w systemie...", "Analyzing system bloat and neckbeard index..."},
        {35, "Optymalizowanie modułów higieny osobistej...", "Optimizing hygiene modules and audio drivers..."},
        {65, "Weryfikacja binarnego bóstwa C++20 FATfetch...", "Verifying precompiled C++20 FATfetch binary..."},
        {85, "Pobieranie 4500 kcal zapasów Monster Energy z AUR...", "Downloading 4500 kcal Monster Energy crates from AUR..."},
        {100, "Instalacja zakończona sukcesem!", "Installation completed successfully!"}
    };

    if (!fs::exists("./fatfetch")) {
        system("make fatfetch >/dev/null 2>&1");
    }

    for (const auto& step : steps) {
        std::string msg = (lang == Language::PL) ? step.msgPL : step.msgEN;
        std::cout << "\r\033[K\033[1;36m[" << step.percent << "%]\033[0m \033[1;37m" << msg << "\033[0m\n";
        sleepMs(60);
    }

    // File copy
    std::string targetPath = "./fatfetch";
    if (!installDir.empty()) {
        targetPath = installDir + "/fatfetch";
        if (requiresSudo) {
            std::string cmd = "sudo mkdir -p " + installDir + " && sudo cp -f ./fatfetch " + targetPath + " && sudo chmod 755 " + targetPath;
            system(cmd.c_str());
        } else {
            std::error_code ec;
            fs::create_directories(installDir, ec);
            fs::copy_file("./fatfetch", targetPath, fs::copy_options::overwrite_existing, ec);
            chmod(targetPath.c_str(), 0755);
        }

        if (lang == Language::PL) {
            std::cout << "\n\033[1;32m✔ Zainstalowano binarkę w: \033[1;37m" << targetPath << "\033[0m\n";
        } else {
            std::cout << "\n\033[1;32m✔ Binary installed to: \033[1;37m" << targetPath << "\033[0m\n";
        }

        configureShellIntegration(homeDir, targetPath, autoStart, lang);
    }

    std::cout << "\n\033[1;32m════════════════════════════════════════════════════════════════════════════════\033[0m\n";
    if (lang == Language::PL) {
        std::cout << "\033[1;35m      GRATULACJE! FATfetch ZOSTAŁ POMYŚLNIE ZAINSTALOWANY!                      \033[0m\n\n";
        std::cout << "\033[1;33m💡 WSKAZÓWKA:\033[0m Aby użyć komendy w BIEŻĄCYM oknie terminala, wpisz:\n";
        std::cout << "   \033[1;36msource ~/.bashrc\033[0m (lub \033[1;36msource ~/.zshrc\033[0m), albo po prostu otwórz nowe okno terminala!\n";
    } else {
        std::cout << "\033[1;35m      CONGRATULATIONS! FATfetch HAS BEEN SUCCESSFULLY INSTALLED!                \033[0m\n\n";
        std::cout << "\033[1;33m💡 TIP:\033[0m To use the command in the CURRENT terminal window, run:\n";
        std::cout << "   \033[1;36msource ~/.bashrc\033[0m (or \033[1;36msource ~/.zshrc\033[0m), or simply open a new terminal window!\n";
    }
    std::cout << "\033[1;32m════════════════════════════════════════════════════════════════════════════════\033[0m\n\n";

    if (lang == Language::PL) {
        std::cout << "Naciśnij [ENTER], aby odpalić testowy run FATfetch!\n";
    } else {
        std::cout << "Press [ENTER] to launch your first FATfetch run!\n";
    }
    std::getline(std::cin, dummy);

    clearScreen();
    std::string testCmd = targetPath + " --lang=" + LocaleManager::languageToString(lang);
    system(testCmd.c_str());

    return 0;
}
