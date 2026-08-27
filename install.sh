#!/usr/bin/env bash
# ==============================================================================
#  🍔 FATfetch - Universal Installer
# ==============================================================================

set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
cd "$DIR"

# Ensure execute permissions on bundled binaries if present
chmod +x ./fatfetch ./fatfetch-installer 2>/dev/null || true

# If precompiled TUI installer exists, launch directly!
if [[ -f "./fatfetch-installer" && -x "./fatfetch-installer" ]]; then
    exec ./fatfetch-installer "$@"
fi

# If binaries are not compiled yet, show clean message and compile
echo -e "\033[1;36m[FATfetch]\033[0m Inicjalizacja instalatora..."

if command -v g++ &>/dev/null && command -v make &>/dev/null; then
    echo -e "\033[1;33m[FATfetch]\033[0m Kompilowanie C++20 dla Twojego procesora..."
    make -j"$(nproc 2>/dev/null || echo 2)" all
    chmod +x ./fatfetch ./fatfetch-installer 2>/dev/null || true
    if [[ -f "./fatfetch-installer" && -x "./fatfetch-installer" ]]; then
        exec ./fatfetch-installer "$@"
    fi
fi

# Fallback quick shell installer if compiler is not present
clear 2>/dev/null || true
echo -e "\033[1;36m"
cat << 'EOF'
╔════════════════════════════════════════════════════════════════════════════════╗
║              FATfetch - OFICJALNY ARCH LINUX TUI INSTALATOR                    ║
║       "Bo Twój neofetch nie był wystarczająco spasiony ani potężny"            ║
╚════════════════════════════════════════════════════════════════════════════════╝
EOF
echo -e "\033[0m"

echo -e "\033[1;33m[ WYBÓR JĘZYKA / SELECT LANGUAGE ]\033[0m"
echo " 1) Polski 🇵🇱 (Domyślny)"
echo " 2) English 🇺🇸/🇬🇧"
read -rp "Wybór / Choice [1-2] (1): " LANG_CHOICE

if [[ "$LANG_CHOICE" == "2" || "$LANG_CHOICE" == "en" ]]; then
    LANG="en"
    echo -e "\n\033[1;33m[ STEP 1/2: WHO DO YOU WANT TO BE? ]\033[0m"
    echo " 1) 🦣 Arch Chad Big Guy (Default)"
    echo " 2) 🌸 300kg Chonky Femboy (Programming socks + Femboy Palette)"
    echo " 3) 🏳️‍⚧️ Trans Pride Femboy"
    echo " 4) 🦤 Discord Basement Mod"
    read -rp "Select identity [1-4] (1): " PERSONA_CHOICE

    echo -e "\n\033[1;33m[ STEP 2/2: INSTALLATION TARGET ]\033[0m"
    echo " 1) /usr/local/bin/fatfetch (System-wide - Recommended)"
    echo " 2) ~/.local/bin/fatfetch   (User-only)"
    read -rp "Select option [1-2] (1): " TARGET_CHOICE
else
    LANG="pl"
    echo -e "\n\033[1;33m[ KROK 1/2: KIM CHCESZ BYĆ W FATfetch? ]\033[0m"
    echo " 1) 🦣 Arch Chad Grubas (Domyślny)"
    echo " 2) 🌸 300kg Gruby Femboy (Zakolanówki + Paleta Femboy)"
    echo " 3) 🏳️‍⚧️ Trans Pride Femboy"
    echo " 4) 🦤 Discord Mod z piwnicy"
    read -rp "Wybierz tożsamość [1-4] (1): " PERSONA_CHOICE

    echo -e "\n\033[1;33m[ KROK 2/2: LOKALIZACJA INSTALACJI ]\033[0m"
    echo " 1) /usr/local/bin/fatfetch (Dla całego systemu - ZALECANE)"
    echo " 2) ~/.local/bin/fatfetch   (Dla użytkownika)"
    read -rp "Wybierz opcję [1-2] (1): " TARGET_CHOICE
fi

# Set persona
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

# Save config file
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

if [[ "$TARGET_CHOICE" == "2" ]]; then
    BIN_DIR="$HOME/.local/bin"
    SUDO=""
else
    BIN_DIR="/usr/local/bin"
    if [[ $EUID -ne 0 ]]; then
        SUDO="sudo"
    else
        SUDO=""
    fi
fi

$SUDO mkdir -p "$BIN_DIR"
$SUDO cp -f ./fatfetch "$BIN_DIR/fatfetch"
$SUDO chmod 755 "$BIN_DIR/fatfetch"

# Ensure PATH and alias in rc files
for rc in "$HOME/.bashrc" "$HOME/.zshrc" "$HOME/.profile"; do
    if [[ -f "$rc" ]]; then
        if ! grep -q "$HOME/.local/bin" "$rc"; then
            echo -e '\nexport PATH="$HOME/.local/bin:$PATH"' >> "$rc"
        fi
        if ! grep -q "alias fatfetch=" "$rc"; then
            echo -e "alias fatfetch=\"$BIN_DIR/fatfetch\"" >> "$rc"
        fi
    fi
done

if [[ "$LANG" == "pl" ]]; then
    echo -e "\n\033[1;32m✔ FATfetch został pomyślnie zainstalowany w $BIN_DIR/fatfetch!\033[0m"
    echo -e "\033[1;33m💡 WSKAZÓWKA:\033[0m Aby zmienić ustawienia w dowolnym momencie, wpisz: \033[1;36mfatfetch --config\033[0m\n"
else
    echo -e "\n\033[1;32m✔ FATfetch successfully installed to $BIN_DIR/fatfetch!\033[0m"
    echo -e "\033[1;33m💡 TIP:\033[0m To change settings anytime, run: \033[1;36mfatfetch --config\033[0m\n"
fi

"$BIN_DIR/fatfetch"
