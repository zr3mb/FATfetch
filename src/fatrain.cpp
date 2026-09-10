#include "fatrain.hpp"
#include "palettes.hpp"
#include "jokes.hpp"
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
#include <cstdlib>
#include <algorithm>
#include <random>

namespace FATfetch {

static struct termios orig_rain_termios;
static bool rain_raw_active = false;
static volatile bool rain_running = true;

static void disableRainRawMode() {
    if (rain_raw_active) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_rain_termios);
        std::cout << "\033[?25h" << std::flush; // show cursor
        rain_raw_active = false;
    }
}

static void enableRainRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_rain_termios) == -1) return;
    struct termios raw = orig_rain_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    rain_raw_active = true;
    std::cout << "\033[?25l" << std::flush; // hide cursor
}

static void rainSignalHandler(int) {
    rain_running = false;
    disableRainRawMode();
    std::cout << "\033[2J\033[H\033[0m" << std::flush;
    exit(0);
}

enum class ItemType {
    BURGER,
    MONSTER,
    PIEROG,
    PIZZA,
    CHICKEN,
    SHOWER
};

struct RainItem {
    int x;
    double y;
    double speed;
    ItemType type;
    std::string icon;
    int calories;
};

struct ArchCatcher {
    int x;
    int targetX;
    std::string catchText;
    int catchTimer = 0;
    bool isPlayer = false;
    int eatenCount = 0;
};

void FatRain::run(const std::string& initialPalette) {
    signal(SIGINT, rainSignalHandler);
    signal(SIGTERM, rainSignalHandler);

    enableRainRawMode();
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

    bool autopilot = false; // Player mode by default, Space toggles
    int totalCalories = 0;
    int totalBurgers = 0;
    int totalMonsters = 0;
    int rainDensity = 4; // Spawn rate divisor

    // Get terminal size
    struct winsize ws;
    int termCols = 80;
    int termRows = 24;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 30 && ws.ws_row > 15) {
        termCols = ws.ws_col;
        termRows = ws.ws_row;
    }

    // Player & Bot Arch Users at the bottom
    std::vector<ArchCatcher> catchers;
    ArchCatcher player;
    player.x = termCols / 2;
    player.targetX = player.x;
    player.isPlayer = true;
    catchers.push_back(player);

    // AI Bots
    ArchCatcher bot1;
    bot1.x = std::max(8, termCols / 4);
    bot1.targetX = bot1.x;
    catchers.push_back(bot1);

    ArchCatcher bot2;
    bot2.x = std::min(termCols - 12, (termCols * 3) / 4);
    bot2.targetX = bot2.x;
    catchers.push_back(bot2);

    std::vector<RainItem> items;
    int frameTick = 0;

    while (rain_running) {
        // Update terminal dimensions
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 30 && ws.ws_row > 15) {
            termCols = ws.ws_col;
            termRows = ws.ws_row;
        }

        // Non-blocking keyboard input
        char ch;
        int n = read(STDIN_FILENO, &ch, 1);
        if (n > 0) {
            if (ch == 'q' || ch == 'Q' || ch == 27) {
                // Check if escape sequence (arrow keys)
                if (ch == 27) {
                    char seq[2];
                    if (read(STDIN_FILENO, &seq[0], 1) > 0 && read(STDIN_FILENO, &seq[1], 1) > 0) {
                        if (seq[0] == '[') {
                            if (seq[1] == 'D') { // Left arrow
                                catchers[0].x = std::max(6, catchers[0].x - 4);
                                autopilot = false;
                            } else if (seq[1] == 'C') { // Right arrow
                                catchers[0].x = std::min(termCols - 10, catchers[0].x + 4);
                                autopilot = false;
                            }
                        }
                    } else {
                        break; // Esc pressed -> exit
                    }
                } else {
                    break; // q pressed -> exit
                }
            } else if (ch == 'a' || ch == 'A' || ch == 'h' || ch == 'H') {
                catchers[0].x = std::max(6, catchers[0].x - 4);
                autopilot = false;
            } else if (ch == 'd' || ch == 'D' || ch == 'l' || ch == 'L') {
                catchers[0].x = std::min(termCols - 10, catchers[0].x + 4);
                autopilot = false;
            } else if (ch == ' ') {
                autopilot = !autopilot;
            } else if (ch == 'p' || ch == 'P') {
                palIndex = (palIndex + 1) % palNames.size();
            } else if (ch == '+' || ch == '=') {
                if (rainDensity > 1) rainDensity--;
            } else if (ch == '-' || ch == '_') {
                if (rainDensity < 8) rainDensity++;
            }
        }

        Palette currentPal = PaletteManager::getPalette(palNames[palIndex]);

        // Spawn new falling items
        frameTick++;
        if (frameTick % rainDensity == 0) {
            std::uniform_int_distribution<int> xDist(4, termCols - 8);
            std::uniform_int_distribution<int> typeDist(0, 100);

            RainItem item;
            item.x = xDist(rng);
            item.y = 3.0; // below header
            item.speed = 0.55 + (typeDist(rng) % 4) * 0.15;

            int roll = typeDist(rng);
            if (roll < 42) {
                item.type = ItemType::BURGER;
                item.icon = "🍔 [BURGER 6XL]";
                item.calories = 150;
            } else if (roll < 65) {
                item.type = ItemType::MONSTER;
                item.icon = "🥫 [MONSTER WHITE]";
                item.calories = 100;
            } else if (roll < 82) {
                item.type = ItemType::PIEROG;
                item.icon = "🥟 [PIERÓG]";
                item.calories = 120;
            } else if (roll < 92) {
                item.type = ItemType::PIZZA;
                item.icon = "🍕 [PIZZA]";
                item.calories = 140;
            } else {
                item.type = ItemType::SHOWER;
                item.icon = "🚿 [MYDŁO-BLOAT!]";
                item.calories = -250;
                item.speed += 0.2;
            }
            items.push_back(item);
        }

        // AI Autopilot movement for bots & player if autopilot enabled
        for (size_t i = 0; i < catchers.size(); ++i) {
            if (catchers[i].isPlayer && !autopilot) continue;

            // Find closest falling burger/food item
            double bestDist = 9999.0;
            int targetX = catchers[i].x;
            for (const auto& it : items) {
                if (it.type == ItemType::SHOWER) continue; // avoid showers
                if (it.y < termRows - 8 && it.y > 3.0) {
                    double dist = std::abs(it.x - catchers[i].x) + (termRows - it.y);
                    if (dist < bestDist) {
                        bestDist = dist;
                        targetX = it.x;
                    }
                }
            }

            if (targetX < catchers[i].x - 1) {
                catchers[i].x -= 1;
            } else if (targetX > catchers[i].x + 1) {
                catchers[i].x += 1;
            }
            catchers[i].x = std::clamp(catchers[i].x, 6, termCols - 12);
        }

        // Update items position & collision check
        int archTopY = termRows - 6;
        for (auto& it : items) {
            it.y += it.speed;

            // Check collision with any Arch Chad
            if (it.y >= archTopY - 1 && it.y <= archTopY + 2) {
                for (auto& c : catchers) {
                    if (std::abs(it.x - (c.x + 3)) <= 4) { // Hit mouth/arms!
                        it.y = termRows + 10; // Mark for removal

                        if (it.type == ItemType::SHOWER) {
                            c.catchText = "🚿 *SSSSS! MYDŁO!*";
                            c.catchTimer = 12;
                            if (c.isPlayer) totalCalories = std::max(0, totalCalories - 250);
                        } else {
                            if (it.type == ItemType::BURGER) {
                                c.catchText = "🍔 *OM-NOM-NOM!*";
                                if (c.isPlayer) totalBurgers++;
                            } else if (it.type == ItemType::MONSTER) {
                                c.catchText = "🥫 *GŁÓW-GŁÓW!*";
                                if (c.isPlayer) totalMonsters++;
                            } else if (it.type == ItemType::PIEROG) {
                                c.catchText = "🥟 *MNIAM SKWARKI!*";
                            } else {
                                c.catchText = "🍕 *CHRUP!*";
                            }
                            c.catchTimer = 10;
                            c.eatenCount++;
                            if (c.isPlayer) totalCalories += it.calories;
                        }
                    }
                }
            }
        }

        // Remove off-screen items
        items.erase(std::remove_if(items.begin(), items.end(), [&](const RainItem& it) {
            return it.y >= termRows - 1;
        }), items.end());

        // Update timers
        for (auto& c : catchers) {
            if (c.catchTimer > 0) c.catchTimer--;
        }

        // ----------------------------------------------------------------------
        //  RENDER FRAME INTO BUFFER
        // ----------------------------------------------------------------------
        std::vector<std::string> screen(termRows, std::string(termCols, ' '));

        // 1. Header (lines 0-2)
        std::string C_RST = "\033[0m";
        std::string C_CYN = "\033[1;36m";
        std::string C_YEL = "\033[1;33m";
        std::string C_MAG = "\033[1;35m";
        std::string C_GRN = "\033[1;32m";
        std::string C_RED = "\033[1;31m";
        std::string C_WHT = "\033[1;37m";

        std::cout << "\033[H" << std::flush;

        std::cout << C_CYN << "╔════════════════════════════════════════════════════════════════════════════════╗\n"
                  << "║         🍔 FATRAIN - DESZCZ BURGERÓW I MONSTERÓW DLA ARCH USERÓW 🥫            ║\n"
                  << "╚════════════════════════════════════════════════════════════════════════════════╝\n" << C_RST;

        std::cout << "  " << C_YEL << "🍔 Zjedzone: " << totalBurgers << C_RST
                  << "  |  " << C_CYN << "🥫 Monstery: " << totalMonsters << C_RST
                  << "  |  " << C_GRN << "⚡ Kalorie: " << totalCalories << " kcal" << C_RST
                  << "  |  " << C_MAG << "Tryb: " << (autopilot ? "[AUTOPILOT]" : "[STEROWANIE GRACZA]") << C_RST
                  << "  |  [Spacja] Tryb  [q] Wyjdź\033[K\n";

        // Render falling items directly
        // Create 2D grid of items
        std::vector<std::string> lines(termRows - 4, std::string(termCols, ' '));

        for (const auto& it : items) {
            int row = static_cast<int>(it.y) - 3;
            if (row >= 0 && row < (int)lines.size()) {
                int col = it.x;
                for (size_t k = 0; k < it.icon.length() && col + (int)k < termCols; ++k) {
                    lines[row][col + k] = it.icon[k];
                }
            }
        }

        // Render Arch Chads at the bottom (lines termRows - 10 to termRows - 4)
        for (const auto& c : catchers) {
            int bx = c.x;
            int baseRow = lines.size() - 6;
            if (baseRow < 0) baseRow = 0;

            // Catch popup text
            if (c.catchTimer > 0 && baseRow - 1 >= 0) {
                std::string txt = c.catchText;
                for (size_t k = 0; k < txt.length() && bx + (int)k < termCols; ++k) {
                    lines[baseRow - 1][bx + k] = txt[k];
                }
            }

            // Ascii Arch Chad
            std::vector<std::string> chadLines;
            if (c.catchTimer > 0) {
                // Wide open mouth catching
                chadLines = {
                    "  \\( >O< )/  ",
                    "  /| ### |\\  ",
                    ".-'  ===  '-.",
                    "(  350KG BEBECH )",
                    " \\_____________/ ",
                    "   |___| |___|   "
                };
            } else {
                chadLines = {
                    "   /\\___/\\   ",
                    "  (  o O  )  ",
                    "  /| ### |\\  ",
                    ".-'  ===  '-.",
                    "(  350KG BEBECH )",
                    "   |___| |___|   "
                };
            }

            for (size_t r = 0; r < chadLines.size(); ++r) {
                if (baseRow + (int)r < (int)lines.size()) {
                    for (size_t k = 0; k < chadLines[r].length() && bx + (int)k < termCols; ++k) {
                        lines[baseRow + r][bx + k] = chadLines[r][k];
                    }
                }
            }

            // Label under Chad
            std::string label = c.isPlayer ? "[TY - ARCH CHAD]" : "[ARCH BOT]";
            if (baseRow + 5 < (int)lines.size()) {
                for (size_t k = 0; k < label.length() && bx + (int)k < termCols; ++k) {
                    lines[baseRow + 5][bx + k] = label[k];
                }
            }
        }

        // Print lines to terminal
        for (const auto& l : lines) {
            std::cout << l << "\033[K\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(55));
    }

    disableRainRawMode();
    std::cout << "\033[2J\033[H\033[1;32m✔ Zakończono sesję FATRAIN. Twoi Arch Userzy są najedzeni i szczęśliwi!\033[0m\n\n";
}

} // namespace FATfetch
