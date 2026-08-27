#!/usr/bin/env bash
# ==============================================================================
#  🍔 FATfetch - Standalone Universal Fast Installer
# ==============================================================================

set -e

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
cd "$DIR"

# Ensure execute permissions on bundled binaries
chmod +x ./fatfetch ./fatfetch-installer 2>/dev/null || true

# If precompiled TUI installer exists, launch directly!
if [[ -f "./fatfetch-installer" && -x "./fatfetch-installer" ]]; then
    exec ./fatfetch-installer "$@"
fi

# If missing, build quickly with parallel cores
if command -v g++ &>/dev/null && command -v make &>/dev/null; then
    make -j$(nproc 2>/dev/null || echo 2) all >/dev/null 2>&1 || true
    chmod +x ./fatfetch ./fatfetch-installer 2>/dev/null || true
    if [[ -f "./fatfetch-installer" && -x "./fatfetch-installer" ]]; then
        exec ./fatfetch-installer "$@"
    fi
fi

# Fallback quick shell installer
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
    echo -e "\n\033[1;33m[ INSTALLATION TARGET ]\033[0m"
    echo " 1) /usr/local/bin/fatfetch (System-wide - Recommended)"
    echo " 2) ~/.local/bin/fatfetch   (User-only)"
    read -rp "Select option [1-2] (1): " TARGET_CHOICE
else
    LANG="pl"
    echo -e "\n\033[1;33m[ LOKALIZACJA INSTALACJI ]\033[0m"
    echo " 1) /usr/local/bin/fatfetch (Dla całego systemu - ZALECANE)"
    echo " 2) ~/.local/bin/fatfetch   (Dla użytkownika)"
    read -rp "Wybierz opcję [1-2] (1): " TARGET_CHOICE
fi

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
    echo -e "\033[1;33m💡 WSKAZÓWKA:\033[0m Wpisz: \033[1;36msource ~/.bashrc\033[0m lub otwórz nowy terminal!\n"
else
    echo -e "\n\033[1;32m✔ FATfetch successfully installed to $BIN_DIR/fatfetch!\033[0m"
    echo -e "\033[1;33m💡 TIP:\033[0m Run: \033[1;36msource ~/.bashrc\033[0m or open a new terminal!\n"
fi

"$BIN_DIR/fatfetch" --lang "$LANG"
