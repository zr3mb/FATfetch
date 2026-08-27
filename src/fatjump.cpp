#include "fatjump.hpp"
#include "palettes.hpp"
#include "ascii_art.hpp"
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
#include <algorithm>

namespace FATfetch {

static struct termios orig_termios;
static bool raw_mode_active = false;
static volatile bool keep_running = true;

static void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

static void disableRawMode() {
    if (raw_mode_active) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        std::cout << "\033[?25h" << std::flush; // show cursor
        raw_mode_active = false;
    }
}

static void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) return;
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    raw_mode_active = true;
    std::cout << "\033[?25l" << std::flush; // hide cursor
}

static void signalHandler(int) {
    keep_running = false;
    disableRawMode();
    std::cout << "\033[2J\033[H\033[0m" << std::flush;
    exit(0);
}

struct JumpFrame {
    std::vector<std::string> lines;
    std::string phaseName;
    std::string richterLevel;
    std::string jiggleSpeed;
};

static std::vector<JumpFrame> createArchGuyJumpFrames(const Palette& pal) {
    std::string C_RST = "\033[0m";
    std::string C_CYN = pal.ansiColors.empty() ? "\033[1;36m" : pal.ansiColors[0];
    std::string C_PNK = pal.ansiColors.size() > 1 ? pal.ansiColors[1] : "\033[38;5;218m";
    std::string C_SKN = "\033[38;5;216m";
    std::string C_BLK = "\033[38;5;233m";
    std::string C_DGR = "\033[38;5;238m";
    std::string C_BLU = "\033[1;34m";
    std::string C_YEL = "\033[1;33m";
    std::string C_RED = "\033[1;31m";
    std::string C_SWT = "\033[1;36m";
    std::string C_CAN = "\033[1;32m";
    std::string C_CHS = "\033[1;33m";

    std::vector<JumpFrame> frames(8);

    // Frame 0: Deep Compression / Trampoline bends down to max depth
    frames[0].phaseName = "1. GŁĘBOKA KOMPRESJA (MAX NACISK NA TRAMPOLINĘ)";
    frames[0].richterLevel = "9.8 RICHTER (SEJSMICZNY WSTRZĄS)";
    frames[0].jiggleSpeed = "180 Hz (Rezonans falowania)";
    frames[0].lines = {
        "",
        "",
        "",
        "",
        "                   " + C_DGR + ".---." + C_RST + "          " + C_SWT + "💦 💦" + C_RST,
        "                 " + C_SKN + "/ >.< \\" + C_RST + "        " + C_RED + "* ŚCISKANIE MASY! *" + C_RST,
        "                 " + C_SKN + "|  " + C_DGR + "###" + C_SKN + " |" + C_RST,
        "             " + C_BLK + ".---' === '---." + C_RST,
        "           " + C_BLK + "/                 \\" + C_RST + "   " + C_CYN + "   /\\" + C_RST,
        "          " + C_BLK + "/   " + C_SKN + "(  vv GIGA vv  )" + C_BLK + "  \\" + C_CYN + "  /  \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(   KOMPRESJA  )" + C_BLK + "   |" + C_CYN + " / /\\ \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(   ROZLANIE   )" + C_BLK + "   |" + C_CYN + "/ /__\\ \\" + C_RST,
        "          " + C_BLK + "\\   " + C_SKN + "'--------------'" + C_BLK + "  / " + C_CYN + "archlinux" + C_RST,
        "           " + C_SKN + "'---.____________.--'" + C_RST,
        "             " + C_BLU + "(____/        \\____)" + C_RST + "   " + C_YEL + "[UGIĘTE KOLANA]" + C_RST,
        C_YEL + "  ═════════════════\\                  /═════════════════" + C_RST,
        C_YEL + "                    \\________________/" + C_RST + "   " + C_RED + "*NAPINANIE SPRĘŻYN*" + C_RST,
        "                         |      |"
    };

    // Frame 1: Explosion Upwards / Launch from Trampoline!
    frames[1].phaseName = "2. WYSTRZAŁ KINETYCZNY (WYBICIE 300KG W POWIETRZE)";
    frames[1].richterLevel = "8.4 RICHTER (ODEPCHNIĘCIE)";
    frames[1].jiggleSpeed = "240 Hz (Wystrzał w górę)";
    frames[1].lines = {
        "",
        "",
        "                   " + C_SWT + "💦 💦" + C_RST + "  " + C_CAN + "[🥫 MONSTER]" + C_RST,
        "                   " + C_DGR + ".---." + C_RST + "          " + C_CHS + "🧀 *CHRUPKI W POWIETRZU*" + C_RST,
        "                 " + C_SKN + "/ 'O.O'\\" + C_RST + "       " + C_YEL + "* WYBUCH KINETYCZNY! *" + C_RST,
        "                 " + C_SKN + "|  " + C_DGR + "###" + C_SKN + " |" + C_RST,
        "             " + C_BLK + ".---' === '---." + C_RST,
        "           " + C_BLK + "/                 \\" + C_RST + "   " + C_CYN + "   /\\" + C_RST,
        "          " + C_BLK + "/   " + C_SKN + "(  ^^ 300KG ^^  )" + C_BLK + "  \\" + C_CYN + "  /  \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(   WYSTRZAŁ   )" + C_BLK + "   |" + C_CYN + " / /\\ \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(  W GÓRĘ ^^   )" + C_BLK + "   |" + C_CYN + "/ /__\\ \\" + C_RST,
        "          " + C_BLK + "\\   " + C_SKN + "'--------------'" + C_BLK + "  / " + C_CYN + "archlinux" + C_RST,
        "           " + C_SKN + "'---.____________.--'" + C_RST,
        "                 " + C_BLU + "/   ||   \\" + C_RST + "       " + C_YEL + "*ODBICIE OD ZIEMI!*" + C_RST,
        "                " + C_BLU + "(_/  ||    \\_)" + C_RST,
        C_YEL + "  ════════════════════/^^^^^^^^\\════════════════════" + C_RST,
        "                         |      |",
        "                         |      |"
    };

    // Frame 2: Ascending Slow-Mo / Fat layers ripple dynamically
    frames[2].phaseName = "3. WZNOSZENIE W ZWOLNIONYM TEMPIE (FALOWANIE TŁUSZCZU)";
    frames[2].richterLevel = "3.2 RICHTER (W POWIETRZU)";
    frames[2].jiggleSpeed = "160 Hz (Falowanie warstwowe)";
    frames[2].lines = {
        "",
        "         " + C_SWT + "💦" + C_RST + "        " + C_CAN + "[🥫]" + C_RST + "      " + C_CHS + "🧀" + C_RST,
        "                   " + C_DGR + ".---." + C_RST + "          " + C_SWT + "💦 💧" + C_RST,
        "                 " + C_SKN + "/ ~.~ \\" + C_RST + "        " + C_YEL + "* FALOWANIE WARSTW TŁUSZCZU *" + C_RST,
        "                 " + C_SKN + "|  " + C_DGR + "###" + C_SKN + " |" + C_RST,
        "             " + C_BLK + ".---' === '---." + C_RST,
        "           " + C_BLK + "/                 \\" + C_RST + "   " + C_CYN + "   /\\" + C_RST,
        "          " + C_BLK + "/   " + C_SKN + "( << FALOWANIE )" + C_BLK + "  \\" + C_CYN + "  /  \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(   BEBECHA >> )" + C_BLK + "   |" + C_CYN + " / /\\ \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(  SPRĘŻYNUJE  )" + C_BLK + "   |" + C_CYN + "/ /__\\ \\" + C_RST,
        "          " + C_BLK + "\\   " + C_SKN + "'--------------'" + C_BLK + "  / " + C_CYN + "archlinux" + C_RST,
        "           " + C_SKN + "'---.____________.--'" + C_RST,
        "                 " + C_BLU + "/        \\" + C_RST + "      " + C_CYN + "*LEWITACJA MASY*" + C_RST,
        "                " + C_BLU + "(__/    \\__)" + C_RST,
        "",
        C_YEL + "  ══════════════════════════════════════════════════" + C_RST,
        "                         |      |",
        "                         |      |"
    };

    // Frame 3: Apex / Zero-G Transcendence (Floating in Mid-Air)
    frames[3].phaseName = "4. APEX / STAN ZERO-G (MAJESTATYCZNY LEWITUJĄCY BEBECH)";
    frames[3].richterLevel = "0.0 RICHTER (STAN NIEWAŻKOŚCI)";
    frames[3].jiggleSpeed = "60 Hz (Płynna bezwładność w próżni)";
    frames[3].lines = {
        "       " + C_PNK + "✨ ✨" + C_RST + "   " + C_SWT + "💧" + C_RST + "   " + C_CAN + "[🥫 MONSTER]" + C_RST + "   " + C_CHS + "🧀" + C_RST + "   " + C_PNK + "✨ ✨" + C_RST,
        "                   " + C_DGR + ".---." + C_RST + "          " + C_PNK + "♥ TRANSCENDENCJA TŁUSZCZU ♥" + C_RST,
        "                 " + C_SKN + "/ ◕ ‿ ◕ \\" + C_RST + "      " + C_CYN + "* NIEWAŻKOŚĆ 300KG! *" + C_RST,
        "                 " + C_SKN + "|  " + C_DGR + "###" + C_SKN + " |" + C_RST,
        "             " + C_BLK + ".---' === '---." + C_RST,
        "           " + C_BLK + "/                 \\" + C_RST + "   " + C_CYN + "   /\\" + C_RST,
        "          " + C_BLK + "/   " + C_SKN + "( ☁️ BEBECH W ☁️ )" + C_BLK + "  \\" + C_CYN + "  /  \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(  NIEWAŻKOŚCI )" + C_BLK + "   |" + C_CYN + " / /\\ \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(  UNOSI SIĘ   )" + C_BLK + "   |" + C_CYN + "/ /__\\ \\" + C_RST,
        "          " + C_BLK + "\\   " + C_SKN + "'--------------'" + C_BLK + "  / " + C_CYN + "archlinux" + C_RST,
        "           " + C_SKN + "'---.____________.--'" + C_RST,
        "                 " + C_BLU + "/    ||    \\" + C_RST + "     " + C_PNK + "*LEWITUJĄCE FAŁDY*" + C_RST,
        "                " + C_BLU + "(____/  \\____)" + C_RST,
        "",
        "",
        C_YEL + "  ══════════════════════════════════════════════════" + C_RST,
        "                         |      |",
        "                         |      |"
    };

    // Frame 4: Zero-G Lateral Wobble / Slow-mo Ripple
    frames[4].phaseName = "5. OSCYLACJA W APEXIE (FALA POPRZECZNA BEBECHA)";
    frames[4].richterLevel = "0.0 RICHTER (PRZECHYŁ W POWIETRZU)";
    frames[4].jiggleSpeed = "90 Hz (Rezonans boczny)";
    frames[4].lines = {
        "       " + C_SWT + "💦" + C_RST + "     " + C_CAN + "[🥫]" + C_RST + "         " + C_CHS + "🧀" + C_RST + "       " + C_SWT + "💧" + C_RST,
        "                   " + C_DGR + ".---." + C_RST + "          " + C_YEL + "* DRGANIE BIODER I BRZUCHA *" + C_RST,
        "                 " + C_SKN + "/ 'O.o'\\" + C_RST + "       " + C_CYN + "* SPADEK BLISKO! *" + C_RST,
        "                 " + C_SKN + "|  " + C_DGR + "###" + C_SKN + " |" + C_RST,
        "             " + C_BLK + ".---' === '---." + C_RST,
        "           " + C_BLK + "/                 \\" + C_RST + "   " + C_CYN + "   /\\" + C_RST,
        "          " + C_BLK + "/   " + C_SKN + "( >> PRZECHYŁ >> )" + C_BLK + " \\" + C_CYN + "  /  \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(   BOCZNY     )" + C_BLK + "   |" + C_CYN + " / /\\ \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(  WOBBLE-MAX  )" + C_BLK + "   |" + C_CYN + "/ /__\\ \\" + C_RST,
        "          " + C_BLK + "\\   " + C_SKN + "'--------------'" + C_BLK + "  / " + C_CYN + "archlinux" + C_RST,
        "           " + C_SKN + "'---.____________.--'" + C_RST,
        "                 " + C_BLU + "\\    ||    /" + C_RST + "     " + C_YEL + "*PĘD POWROTNY*" + C_RST,
        "                " + C_BLU + "(_/   ||   \\_)" + C_RST,
        "",
        "",
        C_YEL + "  ══════════════════════════════════════════════════" + C_RST,
        "                         |      |",
        "                         |      |"
    };

    // Frame 5: Descending Slow-Mo / Parachute Belly Effect
    frames[5].phaseName = "6. SPADEK GRAWITACYJNY (BEBECH JAKO SPADOCHRON OPOROWY)";
    frames[5].richterLevel = "2.5 RICHTER (OPADANIE MASY)";
    frames[5].jiggleSpeed = "175 Hz (Opór powietrza na brzuch)";
    frames[5].lines = {
        "",
        "",
        "                   " + C_DGR + ".---." + C_RST + "          " + C_SWT + "💦 💦 💧" + C_RST,
        "                 " + C_SKN + "/ O_O \\" + C_RST + "        " + C_RED + "* GRAWITACJA PRZYCIĄGA 300KG! *" + C_RST,
        "                 " + C_SKN + "|  " + C_DGR + "###" + C_SKN + " |" + C_RST,
        "             " + C_BLK + ".---' === '---." + C_RST,
        "           " + C_BLK + "/                 \\" + C_RST + "   " + C_CYN + "   /\\" + C_RST,
        "          " + C_BLK + "/   " + C_SKN + "( ^^ SPADOCHRON )" + C_BLK + " \\" + C_CYN + "  /  \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(  Z BRZUCHA   )" + C_BLK + "   |" + C_CYN + " / /\\ \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(  ŁAPIE WIATR )" + C_BLK + "   |" + C_CYN + "/ /__\\ \\" + C_RST,
        "          " + C_BLK + "\\   " + C_SKN + "'--------------'" + C_BLK + "  / " + C_CYN + "archlinux" + C_RST,
        "           " + C_SKN + "'---.____________.--'" + C_RST,
        "                 " + C_BLU + "/    ||    \\" + C_RST + "     " + C_RED + "*SZYKOWANIE DO LĄDOWANIA*" + C_RST,
        "                " + C_BLU + "(_/   ||    \\_)" + C_RST,
        "",
        C_YEL + "  ══════════════════════════════════════════════════" + C_RST,
        "                         |      |",
        "                         |      |"
    };

    // Frame 6: Pre-Impact Preparation (Thighs tense, eyes wide)
    frames[6].phaseName = "7. FAZA TUŻ PRZED LĄDOWANIEM (NAPIĘCIE UD I PODWOZIA)";
    frames[6].richterLevel = "5.0 RICHTER (KOMPRESJA POWIETRZA)";
    frames[6].jiggleSpeed = "210 Hz (Maksymalne naprężenie)";
    frames[6].lines = {
        "",
        "",
        "",
        "                   " + C_DGR + ".---." + C_RST + "          " + C_SWT + "💦 💦" + C_RST,
        "                 " + C_SKN + "/ >.< \\" + C_RST + "        " + C_RED + "* ZACISKANIE ZĘBÓW! *" + C_RST,
        "                 " + C_SKN + "|  " + C_DGR + "###" + C_SKN + " |" + C_RST,
        "             " + C_BLK + ".---' === '---." + C_RST,
        "           " + C_BLK + "/                 \\" + C_RST + "   " + C_CYN + "   /\\" + C_RST,
        "          " + C_BLK + "/   " + C_SKN + "(  vv WISZĄCY  )" + C_BLK + "  \\" + C_CYN + "  /  \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(   KOLOS W    )" + C_BLK + "   |" + C_CYN + " / /\\ \\" + C_RST,
        "         " + C_BLK + "|    " + C_SKN + "(   DÓŁ vv     )" + C_BLK + "   |" + C_CYN + "/ /__\\ \\" + C_RST,
        "          " + C_BLK + "\\   " + C_SKN + "'--------------'" + C_BLK + "  / " + C_CYN + "archlinux" + C_RST,
        "           " + C_SKN + "'---.____________.--'" + C_RST,
        "                " + C_BLU + "(_____/    \\_____)" + C_RST + "  " + C_RED + "*STYK Z TRAMPOLINĄ!*" + C_RST,
        C_YEL + "  ═════════════════\\                  /═════════════════" + C_RST,
        "                    \\________________/",
        "                         |      |",
        "                         |      |"
    };

    // Frame 7: MAX IMPACT / SEISMIC CRUSH (10.0 Richter shockwave)
    frames[7].phaseName = "8. ULTRA-PIERDOLNIĘCIE / MAKSYMALNY ROZBRZĘK MASY";
    frames[7].richterLevel = "10.0 RICHTER (KATASTROFA SEJSMICZNA)";
    frames[7].jiggleSpeed = "350 Hz (KOSMICZNY JIGGLE-OVERLOAD)";
    frames[7].lines = {
        "",
        "",
        "",
        "",
        "                   " + C_DGR + ".---." + C_RST + "          " + C_RED + "💥 💥 💥 *BUMMMMM!* 💥 💥 💥" + C_RST,
        "                 " + C_SKN + "/ >o< \\" + C_RST + "        " + C_SWT + "💦 💦 💧 💧 💦" + C_RST,
        "                 " + C_SKN + "|  " + C_DGR + "###" + C_SKN + " |" + C_RST,
        "             " + C_BLK + ".---' === '---." + C_RST,
        "           " + C_BLK + "/                 \\" + C_RST + "   " + C_CYN + "   /\\" + C_RST,
        "          " + C_BLK + "/  " + C_SKN + "((( ~~ ULTRA ~~ )))" + C_BLK + " \\" + C_CYN + "  /  \\" + C_RST,
        "         " + C_BLK + "|  " + C_SKN + "(((  JIGGLE WAVE  )))" + C_BLK + " |" + C_CYN + " / /\\ \\" + C_RST,
        "         " + C_BLK + "|  " + C_SKN + "(((  ROZLEW 300KG )))" + C_BLK + " |" + C_CYN + "/ /__\\ \\" + C_RST,
        "          " + C_BLK + "\\   " + C_SKN + "'---------------'" + C_BLK + "  / " + C_CYN + "archlinux" + C_RST,
        "           " + C_SKN + "'---._____________.-'" + C_RST,
        "             " + C_BLU + "(______/     \\______)" + C_RST + "  " + C_RED + "*FALA UDERZENIOWA!*" + C_RST,
        C_RED + "  ═════════════════\\                   /═════════════════" + C_RST,
        C_RED + "                    \\_________________/" + C_RST + "   " + C_YEL + "*SPRĘŻYNY PISZCZĄ!*" + C_RST,
        "                         |       |"
    };

    return frames;
}

void FatJump::run(const std::string& initialPalette, const std::string& initialPersona) {
    (void)initialPersona;
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    enableRawMode();

    std::vector<std::string> palNames = PaletteManager::getPaletteNames();
    size_t palIndex = 0;
    for (size_t i = 0; i < palNames.size(); ++i) {
        if (palNames[i] == initialPalette) {
            palIndex = i;
            break;
        }
    }

    int speedMode = 1; // 0 = Matrix Slow-Mo (250ms), 1 = Normal Slow-Mo (140ms), 2 = Fast Jiggle (60ms)
    std::vector<int> delays = { 240, 130, 60 };
    std::vector<std::string> speedLabels = { "MATRIX ULTRA SLOW-MO (240ms)", "KLASYCZNY SLOW-MO (130ms)", "TURBO JIGGLE SPEED (60ms)" };

    int frameIndex = 0;
    int jumpCounter = 0;

    std::cout << "\033[2J\033[H" << std::flush;

    while (keep_running) {
        Palette currentPal = PaletteManager::getPalette(palNames[palIndex]);
        auto frames = createArchGuyJumpFrames(currentPal);

        // Check for non-blocking key presses
        char ch;
        int n = read(STDIN_FILENO, &ch, 1);
        if (n > 0) {
            if (ch == 'q' || ch == 'Q' || ch == 27) { // q or ESC
                break;
            } else if (ch == ' ') {
                speedMode = (speedMode + 1) % 3;
            } else if (ch == 'p' || ch == 'P') {
                palIndex = (palIndex + 1) % palNames.size();
            } else if (ch == 'r' || ch == 'R') {
                jumpCounter = 0;
            }
        }

        // Draw Frame
        std::cout << "\033[H" << std::flush;

        const auto& curFrame = frames[frameIndex % frames.size()];

        std::string C_CYN = "\033[1;36m";
        std::string C_YEL = "\033[1;33m";
        std::string C_MAG = "\033[1;35m";
        std::string C_GRN = "\033[1;32m";
        std::string C_WHT = "\033[1;37m";
        std::string C_RST = "\033[0m";

        std::cout << C_CYN
                  << "╔════════════════════════════════════════════════════════════════════════════════╗\n"
                  << "║         🍔 FATJUMP - WIDŻET SKACZĄCEGO GRUBASA ARCH LINUX W SLOW-MO 🚀         ║\n"
                  << "║         \"Fizyka falującego tłuszczu, pot i 300kg czystej archowej masy\"        ║\n"
                  << "╚════════════════════════════════════════════════════════════════════════════════╝\n"
                  << C_RST;

        std::cout << " " << C_YEL << "[STEROWANIE]:" << C_RST << " [SPACJA] Prędkość: " << C_GRN << speedLabels[speedMode] << C_RST
                  << "  |  [p] Paleta: " << C_MAG << currentPal.name << C_RST
                  << "  |  [q] Wyjdź\n";
        std::cout << " " << C_WHT << "Licznik skoków: " << C_CYN << jumpCounter << C_WHT
                  << "  |  Sejsmograf: " << "\033[1;31m" << curFrame.richterLevel << C_RST
                  << "  |  Częstotliwość: " << C_YEL << curFrame.jiggleSpeed << C_RST << "\033[K\n";
        std::cout << " " << C_MAG << "Faza: " << C_WHT << curFrame.phaseName << C_RST << "\033[K\n";
        std::cout << " ────────────────────────────────────────────────────────────────────────────────\n";

        // Render Jump Scene
        for (const auto& line : curFrame.lines) {
            std::cout << "  " << line << "\033[K\n";
        }

        std::cout << " ────────────────────────────────────────────────────────────────────────────────\n";
        std::cout << " " << C_CYN << "Paleta barw: " << C_RST << currentPal.previewBar << "\033[K\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(delays[speedMode]));

        frameIndex = (frameIndex + 1) % frames.size();
        if (frameIndex == 0) {
            jumpCounter++;
        }
    }

    disableRawMode();
    clearScreen();
    std::cout << "\033[1;32m✔ Zakończono sesję FATJUMP. Twoje kolana i trampolina ocalały!\033[0m\n\n";
}

} // namespace FATfetch
