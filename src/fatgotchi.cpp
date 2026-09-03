#include "fatgotchi.hpp"
#include "palettes.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <fstream>
#include <cstdlib>
#include <algorithm>

namespace FATfetch {

static struct termios orig_game_termios;
static bool game_raw_mode = false;
static volatile bool game_running = true;

static void disableGameRawMode() {
    if (game_raw_mode) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_game_termios);
        std::cout << "\033[?25h" << std::flush;
        game_raw_mode = false;
    }
}

static void enableGameRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_game_termios) == -1) return;
    struct termios raw = orig_game_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    game_raw_mode = true;
    std::cout << "\033[?25l" << std::flush;
}

static void gameSignalHandler(int) {
    game_running = false;
    disableGameRawMode();
    std::cout << "\033[2J\033[H\033[0m" << std::flush;
    exit(0);
}

static int loadHighScore() {
    const char* home = std::getenv("HOME");
    std::string path = (home ? std::string(home) : "/home/user") + "/.config/fatfetch/hiscore.dat";
    std::ifstream file(path);
    int score = 0;
    if (file.is_open()) {
        file >> score;
    }
    return score;
}

static void saveHighScore(int score) {
    const char* home = std::getenv("HOME");
    std::string path = (home ? std::string(home) : "/home/user") + "/.config/fatfetch/hiscore.dat";
    std::ofstream file(path);
    if (file.is_open()) {
        file << score;
    }
}

static std::string renderBar(double val, int width, const std::string& fillColor, const std::string& warnColor) {
    if (val < 0) val = 0;
    if (val > 100) val = 100;
    int filled = static_cast<int>((val * width) / 100.0);

    std::string color = (val < 25.0 || val > 80.0) ? warnColor : fillColor;
    std::string out = color + "[";
    for (int i = 0; i < width; ++i) {
        if (i < filled) out += "█";
        else if (i == filled) out += "▒";
        else out += "░";
    }
    out += "] \033[1;37m" + std::to_string(static_cast<int>(val)) + "%\033[0m";
    return out;
}

void FatgotchiGame::run() {
    signal(SIGINT, gameSignalHandler);
    signal(SIGTERM, gameSignalHandler);

    enableGameRawMode();
    std::cout << "\033[2J\033[H" << std::flush;

    int highScore = loadHighScore();

    bool inGame = true;
    while (inGame && game_running) {
        double fatLevel = 55.0;       // 0% (Starvation) - 100% (Burst/Vomit)
        double monsterLevel = 60.0;   // 0% (Dehydrated/Sleep) - 100% (Tachycardia)
        int score = 0;
        int secondsSurvived = 0;
        int tickCounter = 0;
        std::string lastActionMsg = "Rozpoczęto opiekę nad Arch Chadem! Karm i nawadniaj!";
        std::string gameOverReason = "";
        bool gameOver = false;

        while (!gameOver && game_running) {
            // Non-blocking keyboard input
            char ch;
            int n = read(STDIN_FILENO, &ch, 1);
            if (n > 0) {
                if (ch == 'q' || ch == 'Q' || ch == 27) { // q or Esc
                    inGame = false;
                    break;
                } else if (ch == 'b' || ch == 'B' || ch == '1') {
                    fatLevel += 12.0;
                    monsterLevel -= 3.0;
                    score += 25;
                    lastActionMsg = "🍔 Zjadł podwójnego Burgera 6XL! (+12% Tłuszczu)";
                } else if (ch == 'm' || ch == 'M' || ch == '2') {
                    monsterLevel += 15.0;
                    fatLevel -= 2.0;
                    score += 25;
                    lastActionMsg = "🥫 Wypił puszkę Monstera Ultra White! (+15% Nawodnienia)";
                } else if (ch == 'p' || ch == 'P' || ch == '3') {
                    fatLevel += 20.0;
                    monsterLevel -= 5.0;
                    score += 45;
                    lastActionMsg = "🥟 Wciągnął michę Pierogów ze skwarkami! (+20% Tłuszczu)";
                } else if (ch == 'k' || ch == 'K' || ch == '4') {
                    fatLevel -= 14.0;
                    monsterLevel -= 12.0;
                    score += 150;
                    lastActionMsg = "💻 Skompilował jądro Zen i Chromium z AUR! (+150 pkt, -14% Tłuszczu)";
                }
            }

            // Decay over time
            tickCounter++;
            if (tickCounter % 5 == 0) {
                double decayMultiplier = 1.0 + (secondsSurvived / 45.0);
                fatLevel -= 1.3 * decayMultiplier;
                monsterLevel -= 1.6 * decayMultiplier;
                score += 10;
            }

            if (tickCounter % 15 == 0) {
                secondsSurvived++;
            }

            // Check Game Over Conditions
            if (fatLevel <= 0.0) {
                gameOver = true;
                gameOverReason = "💀 ZHAŃBIENIE: Twój Arch Chad schudł poniżej 100kg, wziął prysznic i poszedł na etat!";
            } else if (fatLevel >= 100.0) {
                gameOver = true;
                gameOverReason = "🤮 KATASTROFA BEBECHA: Przekarmiłeś grubasa burgerami! Szwy pękły, bebech wybuchł i zrzygał się sosem czosnkowym!";
            } else if (monsterLevel <= 0.0) {
                gameOver = true;
                gameOverReason = "💤 ODWODNIENIE KOFEINOWE: Brak Monstera w krwiobiegu! Chad zasnął i nie zapisał konfiguracji Neovima!";
            } else if (monsterLevel >= 100.0) {
                gameOver = true;
                gameOverReason = "⚡ PRZEDAWKOWANIE MONSTERA: 800mg kofeiny wywołało tachykardię i błąd Segmentation Fault (core dumped)!";
            }

            // Render Game Screen
            std::cout << "\033[H" << std::flush;

            std::string C_RST = "\033[0m";
            std::string C_CYN = "\033[1;36m";
            std::string C_MAG = "\033[1;35m";
            std::string C_YEL = "\033[1;33m";
            std::string C_GRN = "\033[1;32m";
            std::string C_RED = "\033[1;31m";
            std::string C_WHT = "\033[1;37m";
            std::string C_SKN = "\033[38;5;216m";
            std::string C_DGR = "\033[38;5;238m";
            std::string C_BLK = "\033[38;5;233m";
            std::string C_BLU = "\033[1;34m";

            std::cout << C_CYN
                      << "╔════════════════════════════════════════════════════════════════════════════════╗\n"
                      << "║           🍔 FATGOTCHI - SYMULTOR OPIEKI NAD ARCH LINUX CHADEM 🥫              ║\n"
                      << "║       \"Karm burgerami, nawadniaj Monsterem i nie dopuść do pęknięcia bebecha\"  ║\n"
                      << "╚════════════════════════════════════════════════════════════════════════════════╝\n"
                      << C_RST;

            std::cout << "  ⏱️  Czas przetrwania: " << C_YEL << secondsSurvived << "s" << C_RST
                      << "   |   ⭐ Wynik: " << C_GRN << score << " pkt" << C_RST
                      << "   |   🏆 Rekord: " << C_MAG << highScore << " pkt" << C_RST << "\033[K\n";
            std::cout << " ────────────────────────────────────────────────────────────────────────────────\n";

            // Status Progress Bars
            std::cout << "  🍔 \033[1mPOZIOM TŁUSZCZU / BEBECH:\033[0m   " << renderBar(fatLevel, 28, "\033[1;33m", "\033[1;31m") << "  (Cel: 40-80%)\033[K\n";
            std::cout << "  🥫 \033[1mNAWODNIENIE MONSTEREM:\033[0m      " << renderBar(monsterLevel, 28, "\033[1;36m", "\033[1;31m") << "  (Cel: 35-80%)\033[K\n";
            std::cout << " ────────────────────────────────────────────────────────────────────────────────\n";

            // Character Ascii Art depending on state
            if (fatLevel > 80.0) {
                // Overfed / Warning
                std::cout << C_RED << "                       .---.           * BLUB... BLUB... BEBECH PĘKA! *" << C_RST << "\033[K\n";
                std::cout << C_SKN << "                     / >o< \\         " << C_RED << "💦 *ZA DUŻO BURGERÓW! ZARAZ ZWMIOTUJĘ!*" << C_RST << "\033[K\n";
                std::cout << C_SKN << "                     | " << C_DGR << "###" << C_SKN << " |" << C_RST << "\033[K\n";
                std::cout << C_BLK << "                 .---' === '---." << C_RST << "\033[K\n";
                std::cout << C_BLK << "               /                 \\" << C_CYN << "   /\\" << C_RST << "\033[K\n";
                std::cout << C_BLK << "              /  " << C_RED << "( !!! 450KG !!! )" << C_BLK << " \\" << C_CYN << "  /  \\" << C_RST << "\033[K\n";
                std::cout << C_BLK << "             |   " << C_RED << "(  BEBECH ROZLAN)" << C_BLK << "  |" << C_CYN << " / /\\ \\" << C_RST << "\033[K\n";
                std::cout << C_BLK << "             |   " << C_RED << "( SZWY TRZESZCZĄ)" << C_BLK << "  |" << C_CYN << "/ /__\\ \\" << C_RST << "\033[K\n";
                std::cout << C_BLK << "              \\  " << C_RED << "'---------------'" << C_BLK << " / " << C_CYN << "archlinux" << C_RST << "\033[K\n";
                std::cout << C_SKN << "               '---.___________.-'" << C_RST << "\033[K\n";
                std::cout << C_BLU << "                    /    ||   \\" << C_RST << "\033[K\n";
                std::cout << C_BLU << "                   (____/  \\___)" << C_RST << "\033[K\n";
            } else if (fatLevel < 25.0 || monsterLevel < 25.0) {
                // Starving / Dying
                std::cout << C_DGR << "                       .---.           " << C_YEL << "* SŁABNĘ... DAJ MI MONSTERA I BURGERA... *" << C_RST << "\033[K\n";
                std::cout << C_SKN << "                     / x.x \\         " << C_RED << "💧 *CHUDNĘ! MÓJ ARCH TRACI MOC!*" << C_RST << "\033[K\n";
                std::cout << C_SKN << "                     | " << C_DGR << "###" << C_SKN << " |" << C_RST << "\033[K\n";
                std::cout << C_BLK << "                 .---' === '---." << C_RST << "\033[K\n";
                std::cout << C_BLK << "               /                 \\" << C_CYN << "   /\\" << C_RST << "\033[K\n";
                std::cout << C_BLK << "              /   " << C_YEL << "(  ZAPADNIĘTY )" << C_BLK << " \\" << C_CYN << "  /  \\" << C_RST << "\033[K\n";
                std::cout << C_BLK << "             |    " << C_YEL << "(    BEBECH   )" << C_BLK << "  |" << C_CYN << " / /\\ \\" << C_RST << "\033[K\n";
                std::cout << C_BLK << "              \\   " << C_YEL << "'------------'" << C_BLK << "  /" << C_CYN << "/ /__\\ \\" << C_RST << "\033[K\n";
                std::cout << C_SKN << "               '---.___________.-'" << C_CYN << "  archlinux" << C_RST << "\033[K\n";
                std::cout << C_BLU << "                    /    ||   \\" << C_RST << "\033[K\n";
                std::cout << C_BLU << "                   (___/    \\___)" << C_RST << "\033[K\n";
            } else {
                // Healthy, thriving 350kg Chad!
                std::cout << C_DGR << "                       .---.           " << C_GRN << "♥ 300KG ARCH LINUX CHAD W PEŁNI SIŁ ♥" << C_RST << "\033[K\n";
                std::cout << C_SKN << "                     / ◕ ‿ ◕ \\       " << C_CYN << "🥫 *Monster w ręku, Burger w zębach*" << C_RST << "\033[K\n";
                std::cout << C_SKN << "                     | " << C_DGR << "###" << C_SKN << " |" << C_RST << "\033[K\n";
                std::cout << C_BLK << "                 .---' === '---." << C_RST << "\033[K\n";
                std::cout << C_BLK << "               /                 \\" << C_CYN << "   /\\" << C_RST << "\033[K\n";
                std::cout << C_BLK << "              /   " << C_SKN << "( ^^ 350KG ^^ )" << C_BLK << " \\" << C_CYN << "  /  \\" << C_RST << "\033[K\n";
                std::cout << C_BLK << "             |    " << C_SKN << "(   IDEALNY   )" << C_BLK << "  |" << C_CYN << " / /\\ \\" << C_RST << "\033[K\n";
                std::cout << C_BLK << "             |    " << C_SKN << "(   BEBECH    )" << C_BLK << "  |" << C_CYN << "/ /__\\ \\" << C_RST << "\033[K\n";
                std::cout << C_BLK << "              \\   " << C_SKN << "'------------'" << C_BLK << "  / " << C_CYN << "archlinux" << C_RST << "\033[K\n";
                std::cout << C_SKN << "               '---.___________.-'" << C_RST << "\033[K\n";
                std::cout << C_BLU << "                    /    ||   \\" << C_RST << "\033[K\n";
                std::cout << C_BLU << "                   (____/  \\____)" << C_RST << "\033[K\n";
            }

            std::cout << " ────────────────────────────────────────────────────────────────────────────────\n";
            std::cout << "  📢 Komunikat: " << C_WHT << lastActionMsg << C_RST << "\033[K\n";
            std::cout << " ────────────────────────────────────────────────────────────────────────────────\n";
            std::cout << "  [1/B] 🍔 Burger 6XL (+12% Tłuszcz)    [2/M] 🥫 Monster White (+15% Nawodnienie)\n";
            std::cout << "  [3/P] 🥟 Pierogi ze skwarkami (+20%)   [4/K] 💻 Kompiluj AUR (+150pkt, -14% Tłuszcz)\n";
            std::cout << "  [Q/Esc] ❌ Wyjdź z gry\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(70));
        }

        if (score > highScore) {
            highScore = score;
            saveHighScore(highScore);
        }

        // GAME OVER SCREEN
        if (gameOver && game_running) {
            std::cout << "\033[2J\033[H" << std::flush;
            std::string C_RED = "\033[1;31m";
            std::string C_YEL = "\033[1;33m";
            std::string C_GRN = "\033[1;32m";
            std::string C_WHT = "\033[1;37m";
            std::string C_RST = "\033[0m";

            std::cout << C_RED
                      << "╔════════════════════════════════════════════════════════════════════════════════╗\n"
                      << "║                         💀  G A M E   O V E R  💀                              ║\n"
                      << "╚════════════════════════════════════════════════════════════════════════════════╝\n"
                      << C_RST << "\n";

            if (fatLevel >= 100.0) {
                // Burst & Vomit Animation
                std::cout << "\033[38;5;118m"
                          << "                🤮 💥 💥 💥 * R Z Y G & B E B E C H   B O O M * 💥 💥 💥 🤮\n"
                          << "                   /\\___/\\      💦 💧 🤮 🤮 🤮 💧\n"
                          << "                  ( > o < )  /\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n"
                          << "                 /|  ###  |\\  \\\\\\\\  [10 LITRÓW SOSU CZOSNKOWEGO] \\\\\\\\\n"
                          << "              .---'  ===  '---. \\\\\\\\  [ZALAŁO KLAWIATURĘ MECHANICZNĄ] \\\\\n"
                          << "             /    * EKSPLOZJA *  \\  \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n"
                          << "\033[0m\n";
            }

            std::cout << "  " << C_RED << gameOverReason << C_RST << "\n\n";
            std::cout << "  ⏱️  Czas przetrwania:  " << C_YEL << secondsSurvived << " sekund" << C_RST << "\n";
            std::cout << "  ⭐ Twój wynik:        " << C_GRN << score << " punktów" << C_RST << "\n";
            std::cout << "  🏆 Najlepszy rekord:  " << C_WHT << highScore << " punktów" << C_RST << "\n\n";

            std::cout << " ────────────────────────────────────────────────────────────────────────────────\n";
            std::cout << "  [R/Spacja] 🔄 Zagraj ponownie     [Q/Esc] ❌ Wyjdź do terminala\n";
            std::cout << " ────────────────────────────────────────────────────────────────────────────────\n";

            while (game_running) {
                char ch;
                int n = read(STDIN_FILENO, &ch, 1);
                if (n > 0) {
                    if (ch == 'r' || ch == 'R' || ch == ' ') {
                        break; // Restart loop
                    } else if (ch == 'q' || ch == 'Q' || ch == 27) {
                        inGame = false;
                        break;
                    }
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
    }

    disableGameRawMode();
    std::cout << "\033[2J\033[H\033[1;32m✔ Zakończono grę FATGOTCHI. Twój Arch Chad przesyła pozdrowienia!\033[0m\n\n";
}

} // namespace FATfetch
