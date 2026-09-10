#include "fatfight.hpp"
#include "palettes.hpp"
#include "jokes.hpp"
#include "locales.hpp"
#include "config.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>

namespace FATfetch {

static struct termios orig_fight_termios;
static bool fight_raw_active = false;
static volatile bool fight_running = true;

static void disableFightRawMode() {
    if (fight_raw_active) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_fight_termios);
        std::cout << "\033[?25h\033[0m" << std::flush;
        fight_raw_active = false;
    }
}

static void enableFightRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_fight_termios) == -1) return;
    struct termios raw = orig_fight_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    fight_raw_active = true;
    std::cout << "\033[?25l" << std::flush;
}

static void fightSignalHandler(int) {
    fight_running = false;
    disableFightRawMode();
    std::cout << "\033[2J\033[H\033[0m" << std::flush;
    exit(0);
}

struct FightRecords {
    int totalBattles = 0;
    int bossesDefeated = 0;
    int victories = 0;
    long long bloatPurgedMB = 0;
};

static FightRecords loadRecords() {
    FightRecords rec;
    const char* home = std::getenv("HOME");
    std::string path = (home ? std::string(home) : "/home/user") + "/.config/fatfetch/fight_records.dat";
    std::ifstream file(path);
    if (file.is_open()) {
        file >> rec.totalBattles >> rec.bossesDefeated >> rec.victories >> rec.bloatPurgedMB;
    }
    return rec;
}

static void saveRecords(const FightRecords& rec) {
    const char* home = std::getenv("HOME");
    std::string path = (home ? std::string(home) : "/home/user") + "/.config/fatfetch/fight_records.dat";
    std::ofstream file(path);
    if (file.is_open()) {
        file << rec.totalBattles << " " << rec.bossesDefeated << " " << rec.victories << " " << rec.bloatPurgedMB << "\n";
    }
}

struct BossMove {
    std::string namePl;
    std::string nameEn;
    int minDmg;
    int maxDmg;
    int drainEnergy;
    int healBoss;
    int stunChance;
    std::string quotePl;
    std::string quoteEn;
};

struct Boss {
    std::string namePl;
    std::string nameEn;
    std::string titlePl;
    std::string titleEn;
    int maxHp;
    int currentHp;
    int defense;
    long long bloatSizeMB;
    std::vector<std::string> asciiArt;
    std::vector<BossMove> moves;
};

static std::string renderHealthBar(int cur, int max, int width, const std::string& colorHigh, const std::string& colorLow) {
    if (cur < 0) cur = 0;
    if (max <= 0) max = 1;
    if (cur > max) cur = max;
    int filled = (cur * width) / max;
    double pct = (double)cur / max;

    std::string color = (pct < 0.3) ? colorLow : colorHigh;
    std::string out = color + "[";
    for (int i = 0; i < width; ++i) {
        if (i < filled) out += "█";
        else if (i == filled) out += "▒";
        else out += "░";
    }
    out += "] \033[1;37m" + std::to_string(cur) + "/" + std::to_string(max) + "\033[0m";
    return out;
}

static std::string renderEnergyBar(int cur, int max, int width) {
    if (cur < 0) cur = 0;
    if (max <= 0) max = 1;
    if (cur > max) cur = max;
    int filled = (cur * width) / max;

    std::string out = "\033[1;36m[";
    for (int i = 0; i < width; ++i) {
        if (i < filled) out += "█";
        else out += "░";
    }
    out += "] \033[1;37m" + std::to_string(cur) + "/" + std::to_string(max) + " 🥫\033[0m";
    return out;
}

static std::vector<Boss> initBosses() {
    std::vector<Boss> bosses;

    // Boss 1: Ubuntu Snap Demon
    {
        Boss b;
        b.namePl = "Ubuntu Snap Demon";
        b.nameEn = "Ubuntu Snap Demon";
        b.titlePl = "Władca Wolnego Uruchamiania & 500MB Kalkulatorów";
        b.titleEn = "Lord of Slow Startup & 500MB Calculators";
        b.maxHp = 320;
        b.currentHp = 320;
        b.defense = 6;
        b.bloatSizeMB = 4500;
        b.asciiArt = {
            "        .-------.",
            "       /  SNAP   \\",
            "      |  (o) (o)  |",
            "       \\   ---   /",
            "      .-'-------'-.",
            "     / / Canonical \\ \\",
            "    / /|  /dev/    |\\ \\",
            "   ( ( |  loop42   | ) )",
            "    \\ \\ \\_________/ / /",
            "     \\_\\           /_/"
        };
        b.moves = {
            {
                "Instalacja Snapa Kalkulatora (512 MB)",
                "Installing Snap Calculator (512 MB)",
                30, 45, 0, 0, 0,
                "Zainstaluj kalkulator, zajmuje tylko 512 MB i startuje w 18 sekund!",
                "Install calculator via snap, takes only 512 MB and starts in 18 seconds!"
            },
            {
                "Canonical Telemetry Probe",
                "Canonical Telemetry Probe",
                25, 38, 0, 30, 0,
                "Canonical serdecznie dziękuje za twoje cenne dane telemetryczne!",
                "Canonical thanks you wholeheartedly for your precious telemetry data!"
            },
            {
                "Zalanie /dev/loop urządzeniami",
                "/dev/loop Devices Flooding",
                35, 52, 10, 0, 0,
                "Montuję kolejne 60 urządzeń loop w twoim /dev! Gdzie twoja czystość Arch?",
                "Mounting 60 more loop devices into your /dev! Where is your Arch cleanliness now?"
            },
            {
                "Zamrożenie Pulpitu przy Aktualizacji",
                "Desktop Freeze During Upgrade",
                40, 58, 15, 0, 25,
                "Błąd konfiguracji dpkg! Twój serwer X11 przestał odpowiadać!",
                "dpkg configuration error! Your display server has stopped responding!"
            }
        };
        bosses.push_back(b);
    }

    // Boss 2: Debian Antique Dinosaur
    {
        Boss b;
        b.namePl = "Debian Antique Dinosaur";
        b.nameEn = "Debian Antique Dinosaur";
        b.titlePl = "Strażnik Jądra 3.2 & Pakietów z Poprzedniej Dekady";
        b.titleEn = "Guardian of Kernel 3.2 & Decade-Old Packages";
        b.maxHp = 480;
        b.currentHp = 480;
        b.defense = 14;
        b.bloatSizeMB = 9800;
        b.asciiArt = {
            "               __",
            "              / _)",
            "     _.----._/ /",
            "    /  Debian /",
            " __/ (o)  (o)/    [GLIBC: 2011]",
            "/__.         |    \"STABLE IS LOVE\"",
            "    | |---| |",
            "    |_|   |_|"
        };
        b.moves = {
            {
                "Blokada Jądra z 2011 Roku",
                "Ancient Kernel 3.2 Lock",
                40, 60, 0, 0, 0,
                "Po co ci jądro 6.x?! Jądro 3.2 działało i nie miało żadnych bajerów!",
                "Why do you need kernel 6.x?! Kernel 3.2 worked and had zero bloat!"
            },
            {
                "Odmowa Sterowników Wi-Fi (Brak Non-Free)",
                "Refuse Wi-Fi Drivers (No Non-Free)",
                35, 50, 25, 0, 0,
                "Twoja karta sieciowa wymaga niewolnego firmware! Użyj kabla RS-232!",
                "Your Wi-Fi NIC requires proprietary firmware! Use an RS-232 serial cable!"
            },
            {
                "Wykład o 10-letnim Uptime",
                "10-Year Uptime Lecture",
                45, 68, 10, 0, 30,
                "Mój serwer działa nieprzerwanie od czasów prezydentury Obamy!",
                "My server has been running since the Obama administration without a reboot!"
            },
            {
                "Zamrożenie Pasków Postępu APT",
                "APT Progress Bar Freeze",
                50, 72, 15, 20, 0,
                "Testujemy ten pakiet w gałęzi unstable jeszcze przez 4 lata!",
                "We will be testing this package in Debian Unstable for 4 more years!"
            }
        };
        bosses.push_back(b);
    }

    // Boss 3: Windows 11 Recall Golem
    {
        Boss b;
        b.namePl = "Windows 11 Recall Golem";
        b.nameEn = "Windows 11 Recall Golem";
        b.titlePl = "Potwór Telemetrii AI, Wymuszonych Restartów i BSOD";
        b.titleEn = "Monster of AI Telemetry, Forced Reboots & BSOD";
        b.maxHp = 680;
        b.currentHp = 680;
        b.defense = 20;
        b.bloatSizeMB = 35000;
        b.asciiArt = {
            "        [  WINDOWS 11  ]",
            "        |  [X]   [_]   |",
            "        +--------------+",
            "       /  ( •_•)        \\   < Copilot AI >",
            "      |  /| 💻 |\\   AI   |  \"Recall active\"",
            "      |   |----|   EDGE  |",
            "      |  /      \\   TPM  |",
            "       \\________2.0_____/",
            "          ||        ||",
            "         /||        ||\\"
        };
        b.moves = {
            {
                "Zrzut Ekranu Total Recall AI",
                "Total Recall AI Screenshot",
                55, 78, 0, 0, 0,
                "Copilot właśnie sfotografował twój terminal, hasła i zawartość lodówki!",
                "Copilot just screenshotted your terminal, root passwords, and your fridge!"
            },
            {
                "Wymuszony Restart o 3:00 w Nocy",
                "Forced 3:00 AM Reboot",
                65, 92, 30, 0, 35,
                "Trwa aktualizacja 1 z 48... Nie wyłączaj komputera w trakcie kompilacji!",
                "Working on update 1 of 48... Do not power off your PC during kernel build!"
            },
            {
                "Agresywny Pop-up Microsoft Edge",
                "Aggressive Microsoft Edge Pop-up",
                48, 65, 10, 35, 0,
                "Microsoft Edge jest o 8000% szybszy od twojego curl i elinksa!",
                "Microsoft Edge is 8000% faster than your terminal curl and links!"
            },
            {
                "Niebieski Ekran Śmierci (BSOD)",
                "Blue Screen of Death (BSOD)",
                80, 115, 20, 0, 0,
                ":( Twój komputer napotkał problem: DRIVER_IRQL_NOT_LESS_OR_EQUAL",
                ":( Your PC ran into a problem: DRIVER_IRQL_NOT_LESS_OR_EQUAL"
            }
        };
        bosses.push_back(b);
    }

    // Boss 4: The Arch Wiki Guru (Final Boss)
    {
        Boss b;
        b.namePl = "Arch Wiki Guru";
        b.nameEn = "Arch Wiki Guru";
        b.titlePl = "Ostateczny Elitista Ricingowy & Pogromca Pytających na Forum";
        b.titleEn = "Ultimate Ricing Elitist & Slayer of Forum Noobs";
        b.maxHp = 880;
        b.currentHp = 880;
        b.defense = 26;
        b.bloatSizeMB = 1024;
        b.asciiArt = {
            "              /\\",
            "             /  \\",
            "            / /\\ \\      [ RTFM LEVEL: OVER 9000 ]",
            "           / /  \\ \\     \"Did you read section 4.2.1?!\"",
            "          / /_  _\\ \\",
            "         / /  \\/  \\ \\    ( ಠ_ಠ )  < ELITIST >",
            "        /_/   ||   \\_\\   /|    |\\",
            "             //\\\\        dvorak-only"
        };
        b.moves = {
            {
                "Fala Uderzeniowa RTFM (Read The F***ing Manual)",
                "RTFM Shockwave (Read The F***ing Manual)",
                70, 98, 0, 0, 0,
                "Dlaczego śmiesz pytać na reddicie?! Wszystko jest opisane w sekcji 12.4.9!",
                "Why dare you ask on Reddit?! It is clearly written in section 12.4.9!"
            },
            {
                "Przepełnienie Stosu Neovim Lua",
                "Neovim Lua Stack Overflow",
                75, 105, 25, 0, 0,
                "Mój plik init.lua ma 14000 linii i sam pisze moje aplikacje!",
                "My init.lua has 14,000 lines and writes my applications autonomously!"
            },
            {
                "Usunięcie X11 i Czysta Otchłań Wayland",
                "Pure Wayland Void (X11 Deprecated)",
                80, 115, 20, 0, 30,
                "Używasz Xorg w 2026 roku?! Twoje okna natychmiast znikają!",
                "Still using Xorg in 2026?! Your windows instantly dissolve into void!"
            },
            {
                "Oczyszczenie z Bloatu (Usuwa Kolory i Czcionki)",
                "Bloat Cleanse (Purges Colors & Fonts)",
                90, 130, 30, 40, 0,
                "Kolory ANSI w terminalu to BLOAT! Prawdziwy haker widzi tylko zera i jedynki!",
                "ANSI colors in terminal are BLOAT! Real hackers only need monochrome TTY!"
            }
        };
        bosses.push_back(b);
    }

    return bosses;
}

static const std::vector<std::string> CHAD_ASCII = {
    "          /\\",
    "         /  \\",
    "        / /\\ \\       ( •̀_•́ )  💪",
    "       / ____ \\      (  :O:  )",
    "      /_/    \\_\\     (  :3:  )",
    "       /|    |\\       /|   |\\",
    "       BEBECH 350KG    POTĘGA"
};

void FatFight::run(const std::string& initialPalette, Language lang) {
    signal(SIGINT, fightSignalHandler);
    signal(SIGTERM, fightSignalHandler);

    enableFightRawMode();
    std::cout << "\033[2J\033[H" << std::flush;

    std::vector<std::string> palNames = PaletteManager::getPaletteNames();
    size_t palIndex = 0;
    for (size_t i = 0; i < palNames.size(); ++i) {
        if (palNames[i] == initialPalette) {
            palIndex = i;
            break;
        }
    }

    std::random_device rd;
    std::mt19937 rng(rd() ^ getpid());

    FightRecords records = loadRecords();
    records.totalBattles++;

    std::vector<Boss> bosses = initBosses();
    size_t currentBossIndex = 0;

    // Player Stats
    int playerMaxHp = 500;
    int playerHp = playerMaxHp;
    int playerMaxEnergy = 100;
    int playerEnergy = playerMaxEnergy;
    int playerDefense = 10;
    double playerMass = 350.5;

    // Combat State
    bool playerBuffDamage = false;
    bool playerEvading = false;
    int playerStunned = 0;
    int bossStunned = 0;
    int bossDefenseReduction = 0;

    std::vector<std::string> combatLog;
    auto addLog = [&](const std::string& msg) {
        combatLog.push_back(msg);
        if (combatLog.size() > 5) {
            combatLog.erase(combatLog.begin());
        }
    };

    if (lang == Language::PL) {
        addLog("\033[1;33m⚔️ Rozpoczęto FATFIGHT: Bitwa o Czystość Systemu Arch Linux!\033[0m");
        addLog("\033[1;36m💡 Wybierz numer ataku [1-6], [P] paleta lub [Q] poddanie się.\033[0m");
    } else {
        addLog("\033[1;33m⚔️ FATFIGHT Started: Battle for Arch Linux Purity!\033[0m");
        addLog("\033[1;36m💡 Choose action [1-6], [P] palette or [Q] retreat.\033[0m");
    }

    bool inGame = true;

    while (inGame && fight_running) {
        Boss& boss = bosses[currentBossIndex];
        Palette pal = PaletteManager::getPalette(palNames[palIndex]);

        // Terminal Size
        struct winsize ws;
        int termCols = 80;
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 20) {
            termCols = ws.ws_col;
        }

        // Render screen
        std::string buf;
        buf.reserve(8192);
        buf += "\033[H"; // Cursor to top-left

        std::string C_RST = "\033[0m";
        std::string C_WHT = "\033[1;37m";
        std::string C_YEL = "\033[1;33m";
        std::string C_GRN = "\033[1;32m";
        std::string C_RED = "\033[1;31m";
        std::string C_CYN = "\033[1;36m";
        std::string C_MAG = "\033[1;35m";
        std::string C_P1 = pal.ansiColors.size() > 0 ? pal.ansiColors[0] : C_CYN;
        std::string C_P2 = pal.ansiColors.size() > 1 ? pal.ansiColors[1] : C_MAG;
        std::string C_P3 = pal.ansiColors.size() > 2 ? pal.ansiColors[2] : C_YEL;

        // Top Banner
        std::string title = (lang == Language::PL)
            ? "⚔️  FATFIGHT: BITWA O BEBECH I ARCH LINUX  ⚔️  [ETAP " + std::to_string(currentBossIndex + 1) + "/4]"
            : "⚔️  FATFIGHT: BATTLE FOR THE BELLY & ARCH LINUX  ⚔️  [STAGE " + std::to_string(currentBossIndex + 1) + "/4]";

        buf += C_P1 + "╔";
        for (int i = 0; i < termCols - 2; ++i) buf += "═";
        buf += "╗\n║ " + C_WHT + title;
        int spaces = termCols - 4 - (int)title.length() + 8; // Adjust for emoji width approx
        for (int i = 0; i < std::max(0, spaces); ++i) buf += " ";
        buf += C_P1 + " ║\n╚";
        for (int i = 0; i < termCols - 2; ++i) buf += "═";
        buf += "╝" + C_RST + "\n";

        // Status Headers: Left (Player) vs Right (Boss)
        std::string pName = (lang == Language::PL) ? "ARCH CHAD (TY)" : "ARCH CHAD (YOU)";
        std::string bName = (lang == Language::PL) ? boss.namePl : boss.nameEn;
        std::string bTitle = (lang == Language::PL) ? boss.titlePl : boss.titleEn;

        buf += "  " + C_CYN + "★ " + pName + " ★" + C_RST;
        int pad = std::max(10, termCols / 2 - 25);
        for (int i = 0; i < pad; ++i) buf += " ";
        buf += C_RED + "☠ " + bName + " ☠" + C_RST + "\n";

        // HP Bars
        std::string pHpBar = renderHealthBar(playerHp, playerMaxHp, 18, C_GRN, C_RED);
        int effectiveDefense = std::max(0, boss.defense - bossDefenseReduction);
        std::string bHpBar = renderHealthBar(boss.currentHp, boss.maxHp, 18, C_RED, "\033[1;31m");

        buf += "  HP: " + pHpBar;
        for (int i = 0; i < std::max(4, termCols / 2 - 35); ++i) buf += " ";
        buf += "HP: " + bHpBar + "\n";

        // Energy Bar (Player) vs Title (Boss)
        std::string pEnergyBar = renderEnergyBar(playerEnergy, playerMaxEnergy, 14);
        buf += "  🥫: " + pEnergyBar;
        for (int i = 0; i < std::max(4, termCols / 2 - 34); ++i) buf += " ";
        buf += "\033[0;33m" + bTitle + C_RST + "\n";

        // Mass and Bloat Size
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << playerMass << " kg";
        buf += "  🍔 Bebech: " + C_YEL + ss.str() + C_RST;
        if (playerBuffDamage) buf += " " + C_GRN + "[+15% DMG]" + C_RST;
        if (playerEvading) buf += " " + C_CYN + "[UNIKI]" + C_RST;
        for (int i = 0; i < std::max(4, termCols / 2 - 30); ++i) buf += " ";
        buf += "📦 Bloat: " + C_MAG + std::to_string(boss.bloatSizeMB) + " MB" + C_RST;
        buf += " (Obrona: " + std::to_string(effectiveDefense) + ")\n";

        buf += C_P2;
        for (int i = 0; i < termCols; ++i) buf += "─";
        buf += C_RST + "\n";

        // Side-by-Side ASCII Art
        size_t maxArtLines = std::max(CHAD_ASCII.size(), boss.asciiArt.size());
        for (size_t i = 0; i < maxArtLines; ++i) {
            std::string leftLine = (i < CHAD_ASCII.size()) ? CHAD_ASCII[i] : "";
            std::string rightLine = (i < boss.asciiArt.size()) ? boss.asciiArt[i] : "";

            buf += C_P1 + leftLine + C_RST;
            int currentLen = (int)leftLine.length();
            int spacing = std::max(6, termCols / 2 - currentLen);
            for (int s = 0; s < spacing; ++s) buf += " ";
            buf += C_RED + rightLine + C_RST + "\n";
        }

        buf += C_P2;
        for (int i = 0; i < termCols; ++i) buf += "─";
        buf += C_RST + "\n";

        // Combat Log Window (Last 5 events)
        buf += C_YEL + "📜 LOG WALKI / COMBAT LOG:" + C_RST + "\n";
        for (size_t i = 0; i < 5; ++i) {
            if (i < combatLog.size()) {
                buf += "  " + combatLog[i] + "\n";
            } else {
                buf += "  \033[0;90m...\033[0m\n";
            }
        }

        buf += C_P2;
        for (int i = 0; i < termCols; ++i) buf += "─";
        buf += C_RST + "\n";

        // Action Menu
        if (lang == Language::PL) {
            buf += C_WHT + "DOSTĘPNE AKCJE:" + C_RST + "\n";
            buf += " " + C_GRN + "[1] 💥 Bebech Slam" + C_RST + " (0 🥫, 45-75 dmg, 30% szansy na ogłuszenie)\n";
            buf += " " + C_CYN + "[2] 🐧 sudo pacman -Syu" + C_RST + " (25 🥫, 80-120 dmg, niszczy obronę)\n";
            buf += " " + C_YEL + "[3] 🥫 Łyk Białego Monstera" + C_RST + " (+90 HP, +35 🥫, +15% dmg buff)\n";
            buf += " " + C_MAG + "[4] ⚡ AUR Git Compile Bomb" + C_RST + " (45 🥫, 140-200 krytycznych dmg)\n";
            buf += " " + C_CYN + "[5] 📢 \"I Use Arch BTW\"" + C_RST + " (15 🥫, 30 dmg, obniża obronę i dezorientuje)\n";
            buf += " " + C_GRN + "[6] 🛡️ Turlanie Bebechem" + C_RST + " (10 🥫, 75% szansy na uniknięcie ciosu)\n";
            buf += " " + C_WHT + "[P] Paleta (" + pal.name + ")  [Q] Poddaj się" + C_RST + "\n";
            buf += C_YEL + "Wybierz akcję [1-6, P, Q]: " + C_RST;
        } else {
            buf += C_WHT + "AVAILABLE ACTIONS:" + C_RST + "\n";
            buf += " " + C_GRN + "[1] 💥 Belly Slam" + C_RST + " (0 🥫, 45-75 dmg, 30% stun chance)\n";
            buf += " " + C_CYN + "[2] 🐧 sudo pacman -Syu" + C_RST + " (25 🥫, 80-120 dmg, shreds defense)\n";
            buf += " " + C_YEL + "[3] 🥫 Chug White Monster" + C_RST + " (+90 HP, +35 🥫, +15% dmg buff)\n";
            buf += " " + C_MAG + "[4] ⚡ AUR Git Compile Bomb" + C_RST + " (45 🥫, 140-200 massive critical dmg)\n";
            buf += " " + C_CYN + "[5] 📢 \"I Use Arch BTW\"" + C_RST + " (15 🥫, 30 dmg, shreds defense & confuses)\n";
            buf += " " + C_GRN + "[6] 🛡️ Belly Dodge & Roll" + C_RST + " (10 🥫, 75% evade chance next hit)\n";
            buf += " " + C_WHT + "[P] Palette (" + pal.name + ")  [Q] Surrender" + C_RST + "\n";
            buf += C_YEL + "Choose action [1-6, P, Q]: " + C_RST;
        }

        std::cout << buf << std::flush;

        // Read user input
        char c = 0;
        while (fight_running) {
            int n = read(STDIN_FILENO, &c, 1);
            if (n > 0) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }

        if (!fight_running || c == 'q' || c == 'Q') {
            inGame = false;
            break;
        }

        if (c == 'p' || c == 'P') {
            palIndex = (palIndex + 1) % palNames.size();
            continue;
        }

        // Check if player is stunned
        if (playerStunned > 0) {
            playerStunned--;
            if (lang == Language::PL) {
                addLog("\033[1;31m⚡ Jesteś ogłuszony przez bloat! Tracisz tę turę!\033[0m");
            } else {
                addLog("\033[1;31m⚡ You are stunned by bloat! You lose this turn!\033[0m");
            }
            // Skip player action directly to enemy turn
            goto ENEMY_TURN;
        }

        playerEvading = false;

        // Player Actions
        if (c == '1') {
            // Bebech Slam
            std::uniform_int_distribution<int> dmgDist(45, 75);
            int rawDmg = dmgDist(rng);
            if (playerBuffDamage) {
                rawDmg = static_cast<int>(rawDmg * 1.15);
                playerBuffDamage = false;
            }
            int actualDmg = std::max(15, rawDmg - effectiveDefense);
            boss.currentHp -= actualDmg;

            // Screen flash effect for impact
            std::cout << "\033[7m" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            std::cout << "\033[0m" << std::flush;

            std::uniform_int_distribution<int> stunDist(1, 100);
            bool stunSuccess = stunDist(rng) <= 30;
            if (stunSuccess) {
                bossStunned = 1;
            }

            if (lang == Language::PL) {
                addLog("\033[1;32m💥 Uderzyłeś z 350kg Bebecha! Zadałeś " + std::to_string(actualDmg) + " obrażeń!" +
                       (stunSuccess ? " [WRÓG OGŁUSZONY!]" : "") + "\033[0m");
            } else {
                addLog("\033[1;32m💥 You slammed with your 350kg Belly! Dealt " + std::to_string(actualDmg) + " dmg!" +
                       (stunSuccess ? " [ENEMY STUNNED!]" : "") + "\033[0m");
            }
            playerMass += 0.2; // Gaining belly mass
        } else if (c == '2') {
            // sudo pacman -Syu
            if (playerEnergy < 25) {
                if (lang == Language::PL) {
                    addLog("\033[1;31m❌ Za mało Energii (wymaga 25 🥫)! Napij się Monstera!\033[0m");
                } else {
                    addLog("\033[1;31m❌ Not enough Energy (requires 25 🥫)! Drink a Monster!\033[0m");
                }
                continue;
            }
            playerEnergy -= 25;
            std::uniform_int_distribution<int> dmgDist(80, 120);
            int rawDmg = dmgDist(rng);
            if (playerBuffDamage) {
                rawDmg = static_cast<int>(rawDmg * 1.15);
                playerBuffDamage = false;
            }
            int actualDmg = std::max(30, rawDmg - effectiveDefense);
            boss.currentHp -= actualDmg;
            bossDefenseReduction += 8;

            if (lang == Language::PL) {
                addLog("\033[1;36m🐧 sudo pacman -Syu --noconfirm zmiótł przestarzałe pakiety! " + std::to_string(actualDmg) + " dmg! (-8 obrony wroga)\033[0m");
            } else {
                addLog("\033[1;36m🐧 sudo pacman -Syu --noconfirm wiped outdated packages! " + std::to_string(actualDmg) + " dmg! (-8 enemy def)\033[0m");
            }
        } else if (c == '3') {
            // Łyk Białego Monstera
            int healHp = 90;
            int healEnergy = 35;
            playerHp = std::min(playerMaxHp, playerHp + healHp);
            playerEnergy = std::min(playerMaxEnergy, playerEnergy + healEnergy);
            playerBuffDamage = true;

            if (lang == Language::PL) {
                addLog("\033[1;33m🥫 Wypiłeś puszkę Białego Monstera! +90 HP, +35 🥫, +15% buff do kolejnego ataku!\033[0m");
            } else {
                addLog("\033[1;33m🥫 You chugged a White Monster! +90 HP, +35 🥫, +15% dmg buff next turn!\033[0m");
            }
        } else if (c == '4') {
            // AUR Git Compile Bomb
            if (playerEnergy < 45) {
                if (lang == Language::PL) {
                    addLog("\033[1;31m❌ Za mało Energii (wymaga 45 🥫)! Napij się Monstera!\033[0m");
                } else {
                    addLog("\033[1;31m❌ Not enough Energy (requires 45 🥫)! Drink a Monster!\033[0m");
                }
                continue;
            }
            playerEnergy -= 45;
            std::uniform_int_distribution<int> dmgDist(140, 200);
            int rawDmg = dmgDist(rng);
            if (playerBuffDamage) {
                rawDmg = static_cast<int>(rawDmg * 1.15);
                playerBuffDamage = false;
            }
            int actualDmg = std::max(60, rawDmg - effectiveDefense);
            boss.currentHp -= actualDmg;

            // Flash effect
            std::cout << "\033[7m" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
            std::cout << "\033[0m" << std::flush;

            if (lang == Language::PL) {
                addLog("\033[1;35m⚡ 100% CPU AUR COMPILATION BOMB! Przegrzałeś wroga: " + std::to_string(actualDmg) + " KRYTYCZNYCH OBRAŻEŃ!\033[0m");
            } else {
                addLog("\033[1;35m⚡ 100% CPU AUR COMPILATION BOMB! Overheated enemy: " + std::to_string(actualDmg) + " CRITICAL DAMAGE!\033[0m");
            }
        } else if (c == '5') {
            // "I Use Arch BTW"
            if (playerEnergy < 15) {
                if (lang == Language::PL) {
                    addLog("\033[1;31m❌ Za mało Energii (wymaga 15 🥫)!\033[0m");
                } else {
                    addLog("\033[1;31m❌ Not enough Energy (requires 15 🥫)!\033[0m");
                }
                continue;
            }
            playerEnergy -= 15;
            boss.currentHp -= 30;
            bossDefenseReduction += 12;

            std::uniform_int_distribution<int> confDist(1, 100);
            if (confDist(rng) <= 50) {
                bossStunned = 1;
            }

            if (lang == Language::PL) {
                addLog(std::string("\033[1;36m📢 'I USE ARCH BTW' flex! Wróg zażenowany! -30 HP, -12 obrony!") +
                       (bossStunned ? " [WRÓG ZDEZORIENTOWANY!]" : "") + "\033[0m");
            } else {
                addLog(std::string("\033[1;36m📢 'I USE ARCH BTW' flex! Enemy cringe overload! -30 HP, -12 def!") +
                       (bossStunned ? " [ENEMY CONFUSED!]" : "") + "\033[0m");
            }
        } else if (c == '6') {
            // Turlanie Bebechem
            if (playerEnergy < 10) {
                if (lang == Language::PL) {
                    addLog("\033[1;31m❌ Za mało Energii (wymaga 10 🥫)!\033[0m");
                } else {
                    addLog("\033[1;31m❌ Not enough Energy (requires 10 🥫)!\033[0m");
                }
                continue;
            }
            playerEnergy -= 10;
            playerEvading = true;

            if (lang == Language::PL) {
                addLog("\033[1;32m🛡️ Przyjąłeś pozycję toczną 350kg! 75% szansy na uniknięcie kolejnego ciosu!\033[0m");
            } else {
                addLog("\033[1;32m🛡️ 350kg rolling defense stance active! 75% chance to dodge next attack!\033[0m");
            }
        } else {
            // Invalid key, ignore
            continue;
        }

        // Check if boss died
        if (boss.currentHp <= 0) {
            boss.currentHp = 0;
            records.bossesDefeated++;
            records.bloatPurgedMB += boss.bloatSizeMB;
            saveRecords(records);

            // Victory over boss animation / dialogue
            std::cout << "\033[2J\033[H" << std::flush;
            std::cout << "\n\n\033[1;32m";
            if (lang == Language::PL) {
                std::cout << "  ╔════════════════════════════════════════════════════════════════════════╗\n"
                          << "  ║             🏆 " << boss.namePl << " ZOSTAŁ POKONANY! 🏆             \n"
                          << "  ║   Pomyślnie wyczyszczono " << boss.bloatSizeMB << " MB zbędnego bloatu z Twojego dysku!   \n"
                          << "  ╚════════════════════════════════════════════════════════════════════════╝\n"
                          << "\033[0m\n";
            } else {
                std::cout << "  ╔════════════════════════════════════════════════════════════════════════╗\n"
                          << "  ║             🏆 " << boss.nameEn << " HAS BEEN DEFEATED! 🏆             \n"
                          << "  ║   Successfully purged " << boss.bloatSizeMB << " MB of useless bloat from your disk!   \n"
                          << "  ╚════════════════════════════════════════════════════════════════════════╝\n"
                          << "\033[0m\n";
            }

            // Stat boosts for defeating boss
            playerMaxHp += 50;
            playerHp = playerMaxHp; // Full heal
            playerMaxEnergy += 20;
            playerEnergy = playerMaxEnergy;
            playerDefense += 5;
            playerMass += 15.0;

            if (lang == Language::PL) {
                std::cout << "  \033[1;33m🎁 NAGRODY ZA ZWYCIĘSTWO:\033[0m\n"
                          << "   • Pełne wyleczenie HP i Energii!\n"
                          << "   • Maksymalne HP wzrosło do: \033[1;32m" << playerMaxHp << "\033[0m\n"
                          << "   • Pojemność na Monstera wzrosła do: \033[1;36m" << playerMaxEnergy << "\033[0m\n"
                          << "   • Bebech urósł o +15kg! Nowa waga: \033[1;33m" << playerMass << " kg\033[0m\n\n";
            } else {
                std::cout << "  \033[1;33m🎁 VICTORY REWARDS:\033[0m\n"
                          << "   • Full HP and Energy restoration!\n"
                          << "   • Max HP increased to: \033[1;32m" << playerMaxHp << "\033[0m\n"
                          << "   • Monster Energy cap increased to: \033[1;36m" << playerMaxEnergy << "\033[0m\n"
                          << "   • Belly mass grew by +15kg! New weight: \033[1;33m" << playerMass << " kg\033[0m\n\n";
            }

            currentBossIndex++;
            if (currentBossIndex >= bosses.size()) {
                // Game Finished! Ultimate Victory
                records.victories++;
                saveRecords(records);

                std::cout << "\033[1;35m";
                if (lang == Language::PL) {
                    std::cout << "  ★════════════════════════════════════════════════════════════════════════★\n"
                              << "  ★          👑 GRATULACJE! POKONAŁEŚ WSZYSTKIE DEMONY BLOATU! 👑          ★\n"
                              << "  ★    Twój system Arch Linux jest w 100% czysty, a Twój Bebech Niezwyciężony!   ★\n"
                              << "  ★    Wyczyszczony Bloat: " << records.bloatPurgedMB << " MB | Waga końcowa: " << playerMass << " kg          ★\n"
                              << "  ★════════════════════════════════════════════════════════════════════════★\n\033[0m\n"
                              << "  Naciśnij dowolny klawisz, aby zakończyć...";
                } else {
                    std::cout << "  ★════════════════════════════════════════════════════════════════════════★\n"
                              << "  ★          👑 CONGRATULATIONS! YOU DEFEATED ALL BLOAT DEMONS! 👑         ★\n"
                              << "  ★     Your Arch Linux is 100% pure and your Belly is Unstoppable!        ★\n"
                              << "  ★    Total Bloat Purged: " << records.bloatPurgedMB << " MB | Final Weight: " << playerMass << " kg         ★\n"
                              << "  ★════════════════════════════════════════════════════════════════════════★\n\033[0m\n"
                              << "  Press any key to exit...";
                }

                char dummy;
                read(STDIN_FILENO, &dummy, 1);
                inGame = false;
                break;
            }

            // Next Stage Prompt
            std::cout << (lang == Language::PL
                ? "  Naciśnij [SPACJĘ], aby przejść do następnego Bossa..."
                : "  Press [SPACE] to face the next Boss...");
            std::cout << std::flush;

            bossDefenseReduction = 0;
            bossStunned = 0;
            playerStunned = 0;
            combatLog.clear();

            while (fight_running) {
                char nextKey = 0;
                if (read(STDIN_FILENO, &nextKey, 1) > 0) {
                    if (nextKey == ' ' || nextKey == '\n' || nextKey == '\r') break;
                    if (nextKey == 'q' || nextKey == 'Q') { inGame = false; break; }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            std::cout << "\033[2J\033[H" << std::flush;
            continue;
        }

ENEMY_TURN:
        // Boss Turn
        if (bossStunned > 0) {
            bossStunned--;
            if (lang == Language::PL) {
                addLog("\033[1;33m🌀 " + boss.namePl + " jest ogłuszony i nie może zaatakować!\033[0m");
            } else {
                addLog("\033[1;33m🌀 " + boss.nameEn + " is stunned and cannot attack!\033[0m");
            }
            continue;
        }

        // Pick random boss move
        std::uniform_int_distribution<size_t> moveDist(0, boss.moves.size() - 1);
        BossMove bMove = boss.moves[moveDist(rng)];

        // Check player evasion
        if (playerEvading) {
            std::uniform_int_distribution<int> evadeDist(1, 100);
            if (evadeDist(rng) <= 75) {
                if (lang == Language::PL) {
                    addLog("\033[1;32m💨 UNIK! Przetoczyłeś 350kg bebecha pod atakiem: [" + bMove.namePl + "]!\033[0m");
                } else {
                    addLog("\033[1;32m💨 DODGE! You rolled your 350kg belly under attack: [" + bMove.nameEn + "]!\033[0m");
                }
                playerEvading = false;
                continue;
            }
            playerEvading = false;
        }

        std::uniform_int_distribution<int> dmgDist(bMove.minDmg, bMove.maxDmg);
        int bossDmg = std::max(10, dmgDist(rng) - playerDefense);
        playerHp -= bossDmg;

        if (bMove.drainEnergy > 0) {
            playerEnergy = std::max(0, playerEnergy - bMove.drainEnergy);
        }
        if (bMove.healBoss > 0) {
            boss.currentHp = std::min(boss.maxHp, boss.currentHp + bMove.healBoss);
        }
        if (bMove.stunChance > 0) {
            std::uniform_int_distribution<int> stunRoll(1, 100);
            if (stunRoll(rng) <= bMove.stunChance) {
                playerStunned = 1;
            }
        }

        std::string moveName = (lang == Language::PL) ? bMove.namePl : bMove.nameEn;
        std::string quote = (lang == Language::PL) ? bMove.quotePl : bMove.quoteEn;

        addLog("\033[1;31m☠ " + boss.namePl + " używa: [" + moveName + "]! -" + std::to_string(bossDmg) + " HP!\033[0m");
        addLog("\033[0;33m  💬 \"" + quote + "\"\033[0m");

        // Check if player died
        if (playerHp <= 0) {
            playerHp = 0;
            std::cout << "\033[2J\033[H" << std::flush;
            std::cout << "\n\n\033[1;31m";
            if (lang == Language::PL) {
                std::cout << "  ╔════════════════════════════════════════════════════════════════════════╗\n"
                          << "  ║                         💀 GAME OVER 💀                               ║\n"
                          << "  ║   Twój Bebech ugiął się pod ciężarem bloatu i telemetrii!              ║\n"
                          << "  ║   Zostałeś zmuszony do zainstalowania Ubuntu ze snapami...             ║\n"
                          << "  ╚════════════════════════════════════════════════════════════════════════╝\n\033[0m\n"
                          << "  Pokonane demony w tej grze: \033[1;33m" << currentBossIndex << "/4\033[0m\n"
                          << "  Naciśnij dowolny klawisz, aby wrócić do terminala...";
            } else {
                std::cout << "  ╔════════════════════════════════════════════════════════════════════════╗\n"
                          << "  ║                         💀 GAME OVER 💀                               ║\n"
                          << "  ║   Your Belly collapsed under the weight of bloat and telemetry!        ║\n"
                          << "  ║   You were forced to install Ubuntu with preinstalled snaps...         ║\n"
                          << "  ╚════════════════════════════════════════════════════════════════════════╝\n\033[0m\n"
                          << "  Demons defeated this run: \033[1;33m" << currentBossIndex << "/4\033[0m\n"
                          << "  Press any key to return to terminal...";
            }
            char dummy;
            read(STDIN_FILENO, &dummy, 1);
            inGame = false;
            break;
        }
    }

    disableFightRawMode();
    std::cout << "\033[2J\033[H\033[0m" << std::flush;
}

} // namespace FATfetch
