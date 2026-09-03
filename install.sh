#!/usr/bin/env bash
# ==============================================================================
#  🍔 FATfetch - Universal Standalone & Source Installer
# ==============================================================================

set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
cd "$DIR"

chmod +x ./fatfetch ./fatfetch-installer 2>/dev/null || true

# If precompiled C++ TUI installer exists, launch directly!
if [[ -f "./fatfetch-installer" && -x "./fatfetch-installer" ]]; then
    exec ./fatfetch-installer "$@"
fi

# If g++ and make exist, compile the C++ installer first
if command -v g++ &>/dev/null && command -v make &>/dev/null; then
    echo -e "\033[1;36m[FATfetch]\033[0m Kompilowanie C++20..."
    make -j"$(nproc 2>/dev/null || echo 2)" all >/dev/null 2>&1 || true
    chmod +x ./fatfetch ./fatfetch-installer 2>/dev/null || true
    if [[ -f "./fatfetch-installer" && -x "./fatfetch-installer" ]]; then
        exec ./fatfetch-installer "$@"
    fi
fi

# ==============================================================================
#  🏃💨 NATIVE BASH 7-SECOND ULTRA-DETAILED BELLY JIGGLE ANIMATION ENGINE
# ==============================================================================

clear 2>/dev/null || true
echo -e "\033[1;36m"
cat << 'EOF'
╔════════════════════════════════════════════════════════════════════════════════╗
║              FATfetch - OFICJALNY ARCH LINUX TUI INSTALATOR                    ║
║       "Bo Twój neofetch nie był wystarczająco spasiony ani potężny"            ║
╚════════════════════════════════════════════════════════════════════════════════╝
EOF
echo -e "\033[0m"

echo -e "\033[1;33m[ WYBÓR JĘZYKA / SELECT LANGUAGE ]\033[0m\n"
echo "  1) Polski 🇵🇱 (Domyślny)"
echo "  2) English 🇺🇸/🇬🇧"
echo ""
read -rp "Wybór / Choice [1-2] (1): " LANG_CHOICE

if [[ "$LANG_CHOICE" == "2" || "$LANG_CHOICE" == "en" || "$LANG_CHOICE" == "EN" ]]; then
    LANG="en"
    echo -e "\n\033[1;33m[ STEP 1/2: WHO DO YOU WANT TO BE IN FATfetch? ]\033[0m\n"
    echo "  1) 🦣 Arch Chad Big Guy (Default)"
    echo "  2) 🌸 300kg Chonky Femboy (Programming socks + Femboy Palette)"
    echo "  3) 🏳️‍⚧️ Trans Pride Femboy"
    echo "  4) 🦤 Discord Basement Mod"
    echo ""
    read -rp "Select identity [1-4] (1): " PERSONA_CHOICE

    echo -e "\n\033[1;33m[ STEP 2/3: INSTALLATION TARGET ]\033[0m\n"
    echo "  1) /usr/local/bin/fatfetch (System-wide - Recommended)"
    echo "  2) ~/.local/bin/fatfetch   (User-only)"
    echo ""
    read -rp "Select option [1-2] (1): " TARGET_CHOICE

    echo -e "\n\033[1;33m[ STEP 3/3: DISCORD RICH PRESENCE (RPC) ]\033[0m\n"
    echo "  Enable Discord RPC daemon autostart on system boot? [Y/n]"
    read -rp "Enable Discord RPC? [Y/n] (y): " RPC_CHOICE
else
    LANG="pl"
    echo -e "\n\033[1;33m[ KROK 1/3: KIM CHCESZ BYĆ W FATfetch? ]\033[0m\n"
    echo "  1) 🦣 Arch Chad Grubas (Domyślny)"
    echo "  2) 🌸 300kg Gruby Femboy (Zakolanówki + Paleta Femboy)"
    echo "  3) 🏳️‍⚧️ Trans Pride Femboy"
    echo "  4) 🦤 Discord Mod z piwnicy"
    echo ""
    read -rp "Wybierz tożsamość [1-4] (1): " PERSONA_CHOICE

    echo -e "\n\033[1;33m[ KROK 2/3: LOKALIZACJA INSTALACJI ]\033[0m\n"
    echo "  1) /usr/local/bin/fatfetch (Dla całego systemu - ZALECANE)"
    echo "  2) ~/.local/bin/fatfetch   (Dla użytkownika)"
    echo ""
    read -rp "Wybierz opcję [1-2] (1): " TARGET_CHOICE

    echo -e "\n\033[1;33m[ KROK 3/3: DISCORD RICH PRESENCE (RPC) ]\033[0m\n"
    echo "  Czy chcesz włączyć demona Discord RPC w autostarcie systemu?"
    read -rp "Włączyć Discord RPC w autostarcie? [T/n] (t): " RPC_CHOICE
fi

LOGO="archguy"
PALETTE="default"
if [[ "$PERSONA_CHOICE" == "2" ]]; then
    LOGO="fatfemboy"
    PALETTE="femboy"
elif [[ "$PERSONA_CHOICE" == "3" ]]; then
    LOGO="fatfemboy"
    PALETTE="trans"
elif [[ "$PERSONA_CHOICE" == "4" ]]; then
    LOGO="discordmod"
    PALETTE="default"
fi

# Save configuration
mkdir -p "$HOME/.config/fatfetch"
cat << EOF > "$HOME/.config/fatfetch/config.conf"
logo = $LOGO
palette = $PALETTE
lang = $LANG
show_joke = true
show_diet = false
color_blocks = true
raw = false
EOF

# ------------------------------------------------------------------------------
#  RUNNING 7-SECOND ANIMATION IN BASH
# ------------------------------------------------------------------------------
printf "\033[?25l\033[2J\033[H"

TOTAL_STEPS=65
for ((step=0; step<=TOTAL_STEPS; step++)); do
    printf "\033[H"
    PERCENT=$(( (step * 100) / TOTAL_STEPS ))
    RUNNER_POS=$(( (step * 34) / TOTAL_STEPS ))
    FRAME=$(( step % 6 ))
    PAD=$(printf '%*s' "$RUNNER_POS" '')

    echo -e "\033[1;36m╔════════════════════════════════════════════════════════════════════════════════╗"
    echo -e "║              FATfetch - OFICJALNY ARCH LINUX MARATON TUI                       ║"
    echo -e "║       \"Bo Twój neofetch nie był wystarczająco spasiony ani potężny\"            ║"
    echo -e "╚════════════════════════════════════════════════════════════════════════════════╝\033[0m"
    echo -e "\033[1;33m [ ETAP 4/4: 7-SEKUNDOWY MARATON BEBECHA ARCH CHADA ]\033[0m\n"

    FINISH_SPACE=$(( 34 - RUNNER_POS ))
    if [ $FINISH_SPACE -lt 1 ]; then FINISH_SPACE=1; fi
    SP=$(printf '%*s' "$FINISH_SPACE" '')

    if [ $FRAME -eq 0 ]; then
        echo -e "${PAD}\033[38;5;238m       .---.       \033[1;36m💦 💦 \033[1;33m*SAPAĆ!*\033[0m${SP}\033[1;37;41m 🏁 META 🏁 \033[0m"
        echo -e "${PAD}\033[38;5;216m     / 'o.o'\\     \033[1;36m💧 💧\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;216m     |  \033[38;5;238m###\033[38;5;216m |   \033[38;5;238m/\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m   .--' === '--' \033[38;5;238m/\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m  /              \\  \033[1;36m   /\\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m /  \033[38;5;216m( ^^ BEBECH ^^ )\033[38;5;233m\\ \033[1;36m  /  \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m|   \033[38;5;216m(  ^ JIGGLE ^  )\033[38;5;233m |\033[1;36m / /\\ \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m \\  \033[38;5;216m(   300KG UP   )\033[38;5;233m/ \033[1;36m/ /__\\ \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m  \\  \033[38;5;216m'------------'\033[38;5;233m /  \033[1;36marchlinux\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;216m   '---.________.-'  \033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m      /        \\     \033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[1;34m    _/          \\_   \033[1;33m*SZUR!*\033[0m"
    elif [ $FRAME -eq 1 ]; then
        echo -e "${PAD}\033[38;5;238m       .---.       \033[1;36m💦 \033[1;31m*DUDU!*\033[0m${SP}\033[1;37;41m 🏁 META 🏁 \033[0m"
        echo -e "${PAD}\033[38;5;216m     / -.-  \\     \033[1;36m💧\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;216m     |  \033[38;5;238m###\033[38;5;216m |  \033[38;5;238m_\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m   .--' === '--' \033[38;5;238m\\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m  /               \\ \033[1;36m  /\\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m /  \033[38;5;216m( ~~ FALA ~~ )\033[38;5;233m  \\\033[1;36m /  \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m|   \033[38;5;216m( vv JIGGLE vv )\033[38;5;233m |\033[1;36m/ /\\ \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m \\  \033[38;5;216m(  OPADANIE   )\033[38;5;233m // /__\\ \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m  \\  \033[38;5;216m'------------'\033[38;5;233m// \033[1;36marchlinux\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;216m   '---.________.-'  \033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m     / /        \\ \\  \033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[1;34m   _/_/          \\_  \033[1;33m*TUP!*\033[0m"
    elif [ $FRAME -eq 2 ]; then
        echo -e "${PAD}\033[38;5;238m       .---.       \033[1;36m💦 💦 \033[1;33m*HUUH!*\033[0m${SP}\033[1;37;41m 🏁 META 🏁 \033[0m"
        echo -e "${PAD}\033[38;5;216m     / >.<  \\     \033[1;36m💧\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;216m     |  \033[38;5;238m###\033[38;5;216m |   \033[38;5;238m\\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m   .--' === '--' \033[38;5;238m\\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m  /              \\  \033[1;36m   /\\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m /  \033[38;5;216m( << FALOWANIE )\033[38;5;233m\\ \033[1;36m  /  \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m|   \033[38;5;216m(   MASY >>    )\033[38;5;233m |\033[1;36m / /\\ \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m \\  \033[38;5;216m(  SPRĘŻYNUJE  )\033[38;5;233m/ \033[1;36m/ /__\\ \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m  \\  \033[38;5;216m'------------'\033[38;5;233m /  \033[1;36marchlinux\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;216m   '---.________.-'  \033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m      /        \\     \033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[1;34m     _/          \\_  \033[1;33m*CHLAP!*\033[0m"
    elif [ $FRAME -eq 3 ]; then
        echo -e "${PAD}\033[38;5;238m       .---.       \033[1;36m💦 💦 \033[1;31m*PUFF!*\033[0m${SP}\033[1;37;41m 🏁 META 🏁 \033[0m"
        echo -e "${PAD}\033[38;5;216m     / O.o  \\     \033[1;36m💧 💧\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;216m     |  \033[38;5;238m###\033[38;5;216m |   \033[38;5;238m/\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m   .--' === '--' \033[38;5;238m/\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m  /              \\  \033[1;36m  /\\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m /  \033[38;5;216m( ^^ 300KG ^^ )\033[38;5;233m \\ \033[1;36m /  \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m|   \033[38;5;216m( ^ PODSKOK ^  )\033[38;5;233m |\033[1;36m/ /\\ \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m \\  \033[38;5;216m(  PĘPEK DRŻY  )\033[38;5;233m/ / /__\\ \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m  \\  \033[38;5;216m'------------'\033[38;5;233m /  \033[1;36marchlinux\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;216m   '---.________.-'  \033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m     \\ \\        / /  \033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[1;34m      \\_        \\_   \033[1;33m*ŁUP!*\033[0m"
    elif [ $FRAME -eq 4 ]; then
        echo -e "${PAD}\033[38;5;238m       .---.       \033[1;36m💦 \033[1;31m*BUMMM!*\033[0m${SP}\033[1;37;41m 🏁 META 🏁 \033[0m"
        echo -e "${PAD}\033[38;5;216m     / >o<  \\     \033[1;36m💧\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;216m     |  \033[38;5;238m###\033[38;5;216m |  \033[38;5;238m_\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m   .--' === '--' \033[38;5;238m\\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m  /               \\ \033[1;36m  /\\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m /  \033[38;5;216m( vv ROZLEW vv )\033[38;5;233m \\\033[1;36m /  \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m|   \033[38;5;216m(  NA BOKI     )\033[38;5;233m |\033[1;36m/ /\\ \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m \\  \033[38;5;216m(  WSTRZĄS 8.5 )\033[38;5;233m// /__\\ \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m  \\  \033[38;5;216m'------------'\033[38;5;233m// \033[1;36marchlinux\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;216m   '---.________.-'  \033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m     / /        \\ \\  \033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[1;34m   _/_/          \\_  \033[1;33m*TRZĘSIENIE!*\033[0m"
    else
        echo -e "${PAD}\033[38;5;238m       .---.       \033[1;36m💦 💦 \033[1;33m*DYCHA!*\033[0m${SP}\033[1;37;41m 🏁 META 🏁 \033[0m"
        echo -e "${PAD}\033[38;5;216m     / 'o.o'\\     \033[1;36m💧\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;216m     |  \033[38;5;238m###\033[38;5;216m |   \033[38;5;238m\\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m   .--' === '--' \033[38;5;238m\\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m  /              \\  \033[1;36m   /\\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m /  \033[38;5;216m( << BEZWŁADNOŚĆ)\033[38;5;233m\\ \033[1;36m  /  \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m|   \033[38;5;216m(   RESONANS   )\033[38;5;233m |\033[1;36m / /\\ \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m \\  \033[38;5;216m(  WOBBLE-MAX  )\033[38;5;233m/ \033[1;36m/ /__\\ \\\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m  \\  \033[38;5;216m'------------'\033[38;5;233m /  \033[1;36marchlinux\033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;216m   '---.________.-'  \033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[38;5;233m      /        \\     \033[0m${SP} \033[1;30;47m█\033[1;37;40m█\033[1;30;47m█\033[0m"
        echo -e "${PAD}\033[1;34m     _/          \\_  \033[1;33m*PĘD!*\033[0m"
    fi

    # Progress bar
    BAR_WIDTH=38
    FILLED=$(( (PERCENT * BAR_WIDTH) / 100 ))
    BAR=""
    for ((b=0; b<BAR_WIDTH; b++)); do
        if [ $b -lt $FILLED ]; then BAR="${BAR}█";
        elif [ $b -eq $FILLED ]; then BAR="${BAR}▒";
        else BAR="${BAR}░"; fi
    done

    echo -e "\n \033[1;36m[${BAR}] \033[1;32m${PERCENT}%\033[0m"
    if [ $PERCENT -lt 25 ]; then
        echo -e " \033[1;37m300kg rusza z miejsca... Bebech osiąga rezonans 140Hz!\033[0m\033[K"
    elif [ $PERCENT -lt 55 ]; then
        echo -e " \033[1;37mKompilowanie C++20... Wylewanie 8 litrów potu na sekundę!\033[0m\033[K"
    elif [ $PERCENT -lt 85 ]; then
        echo -e " \033[1;37mPobieranie zapasów Monster Energy z AUR na trasie biegu...\033[0m\033[K"
    else
        echo -e " \033[1;37mWidzę linię mety! Ostatni potężny zryw bezwładności!\033[0m\033[K"
    fi

    sleep 0.08
done

# Exhausted breathing animation
for ((b=0; b<4; b++)); do
    printf "\033[2J\033[H"
    echo -e "\033[1;36m╔════════════════════════════════════════════════════════════════════════════════╗"
    echo -e "║              FATfetch - OFICJALNY ARCH LINUX MARATON TUI                       ║"
    echo -e "║       \"Bo Twój neofetch nie był wystarczająco spasiony ani potężny\"            ║"
    echo -e "╚════════════════════════════════════════════════════════════════════════════════╝\033[0m"
    echo -e "\033[1;32m [ MARATON ZAKOŃCZONY - GRUBAS DOBIEGŁ I PADA Z WYCIEŃCZENIA! ]\033[0m\n"
    PAD=$(printf '%*s' 34 '')
    echo -e "${PAD}\033[38;5;238m           .---.           \033[1;36m💦 💦 \033[1;31m* HAA... HAAA... UFFF... *\033[0m"
    echo -e "${PAD}\033[38;5;216m          / x.x \\   \033[1;36m💦 💧 \033[1;36m*POT CIURKIEM...*\033[0m"
    echo -e "${PAD}\033[38;5;216m         |  ---  | /       \033[1;33m* 0.00ms TRAWY... *\033[0m"
    echo -e "${PAD}\033[38;5;238m         |  ###  |/        \033[1;36m💧 💧 💧\033[0m"
    echo -e "${PAD}\033[38;5;233m      .---' === '--.       \033[1;36m   /\\\033[0m"
    echo -e "${PAD}\033[38;5;233m     /              \\      \033[1;36m  /  \\\033[0m"
    echo -e "${PAD}\033[38;5;233m    /   \033[38;5;216m(  WISZĄCY BEBECH  )\033[38;5;233m \\     \033[1;36m / /\\ \\\033[0m"
    echo -e "${PAD}\033[38;5;233m   |    \033[38;5;216m(   TOTALNY ZGON   )\033[38;5;233m  |    \033[1;36m/ /__\\ \\\033[0m"
    echo -e "${PAD}\033[38;5;233m   |    \033[38;5;216m( DO SAMEJ ZIEMI   )\033[38;5;233m  |    \033[1;36marchlinux\033[0m"
    echo -e "${PAD}\033[38;5;216m    \\    '----------'   /   \033[0m"
    echo -e "${PAD}\033[38;5;216m     '--.____________.-'    \033[0m"
    echo -e "${PAD}\033[38;5;233m      /  /  |    \\  \\      \033[1;33m[ RĘCE NA KOLANACH ]\033[0m"
    echo -e "${PAD}\033[1;34m     (O_O)  |     (O_O)     \033[0m"
    echo -e "${PAD}\033[1;34m     / /    |      \\ \\      \033[0m"
    echo -e "${PAD}\033[1;34m    (_/     |       \\_)     \033[0m"
    echo -e "\n \033[1;36m[██════════════════════════════════════] \033[1;32m100%\033[0m"
    echo -e " \033[1;32m✔ INSTALACJA ZAKOŃCZONA SUKCESEM!\033[0m"
    sleep 0.38
done

printf "\033[?25h"

# ------------------------------------------------------------------------------
#  COPY BINARIES & FINISH
# ------------------------------------------------------------------------------
mkdir -p "$HOME/.local/bin"
cp -f ./fatfetch "$HOME/.local/bin/fatfetch" 2>/dev/null || true
cp -f ./fatjump "$HOME/.local/bin/fatjump" 2>/dev/null || true
cp -f ./fatrpc "$HOME/.local/bin/fatrpc" 2>/dev/null || true
cp -f ./fatgotchi "$HOME/.local/bin/fatgotchi" 2>/dev/null || true
chmod 755 "$HOME/.local/bin/fatfetch" "$HOME/.local/bin/fatjump" "$HOME/.local/bin/fatrpc" "$HOME/.local/bin/fatgotchi" 2>/dev/null || true

if [[ "$TARGET_CHOICE" == "1" ]]; then
    if [[ $EUID -ne 0 ]]; then
        sudo mkdir -p /usr/local/bin 2>/dev/null || true
        sudo cp -f ./fatfetch ./fatjump ./fatrpc ./fatgotchi /usr/local/bin/ 2>/dev/null || true
        sudo chmod 755 /usr/local/bin/fatfetch /usr/local/bin/fatjump /usr/local/bin/fatrpc /usr/local/bin/fatgotchi 2>/dev/null || true
    else
        mkdir -p /usr/local/bin
        cp -f ./fatfetch ./fatjump ./fatrpc ./fatgotchi /usr/local/bin/
        chmod 755 /usr/local/bin/fatfetch /usr/local/bin/fatjump /usr/local/bin/fatrpc /usr/local/bin/fatgotchi
    fi
fi

# Discord RPC autostart setup
if [[ "$RPC_CHOICE" == "" || "$RPC_CHOICE" == "t" || "$RPC_CHOICE" == "T" || "$RPC_CHOICE" == "y" || "$RPC_CHOICE" == "Y" || "$RPC_CHOICE" == "tak" || "$RPC_CHOICE" == "yes" ]]; then
    "$HOME/.local/bin/fatfetch" --rpc-enable >/dev/null 2>&1 || true
fi

# Ensure PATH in rc files
for rc in "$HOME/.bashrc" "$HOME/.zshrc" "$HOME/.profile"; do
    if [[ -f "$rc" ]]; then
        if ! grep -q "$HOME/.local/bin" "$rc"; then
            echo -e '\nexport PATH="$HOME/.local/bin:$PATH"' >> "$rc"
        fi
        if ! grep -q "alias fatfetch=" "$rc"; then
            echo -e 'alias fatfetch="$HOME/.local/bin/fatfetch"' >> "$rc"
        fi
        if ! grep -q "alias fatjump=" "$rc"; then
            echo -e 'alias fatjump="$HOME/.local/bin/fatjump"' >> "$rc"
        fi
        if ! grep -q "alias fatrpc=" "$rc"; then
            echo -e 'alias fatrpc="$HOME/.local/bin/fatrpc"' >> "$rc"
        fi
        if ! grep -q "alias fatgotchi=" "$rc"; then
            echo -e 'alias fatgotchi="$HOME/.local/bin/fatgotchi"' >> "$rc"
        fi
    fi
done

echo -e "\n\033[1;32m"
echo "╔════════════════════════════════════════════════════════════════════════════════╗"
echo "║                   🏁  INSTALLATION COMPLETE!  🏁                               ║"
echo "║       INSTALACJA FATfetch, FATJUMP, FATRPC I FATGOTCHI ZAKOŃCZONA SUKCESEM!    ║"
echo "╚════════════════════════════════════════════════════════════════════════════════╝"
echo -e "\033[0m"

echo -e "\033[1;33m💡 WSKAZÓWKI:\033[0m"
echo -e "   • Główny fetch: \033[1;36mfatfetch\033[0m"
echo -e "   • Gra Tamagotchi: \033[1;33mfatgotchi\033[0m  (lub \033[1;33mfatfetch --game\033[0m)"
echo -e "   • Skaczący grubas w slow-mo: \033[1;35mfatjump\033[0m"
echo -e "   • Discord RPC daemon: \033[1;34mfatrpc status\033[0m"
echo -e "   • Konfigurator TUI: \033[1;36mfatfetch --config\033[0m"
echo -e "   • Wpisz: \033[1;36msource ~/.bashrc\033[0m (lub otwórz nowy terminal)!\n"

"$HOME/.local/bin/fatfetch"
