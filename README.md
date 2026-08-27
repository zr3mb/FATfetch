# 🍔 FATfetch - The Heaviest System Fetch for Arch Linux (C++)

[![Arch Linux](https://img.shields.io/badge/Arch_Linux-btw_i_use_arch-1793d1?logo=arch-linux)](https://archlinux.org/)
[![Bloat Level](https://img.shields.io/badge/Bloat-OVER_9000-ff0055)](#)
[![Hygiene](https://img.shields.io/badge/Shower-0%20Days-critical)](#)
[![Grass Status](https://img.shields.io/badge/Grass-Untouched-red)](#)
[![Locales](https://img.shields.io/badge/Locales-PL%20%7C%20EN-blue)](#)

> *"Bo Twój zwykły fastfetch/neofetch nie był wystarczająco spasiony, a świat musi wiedzieć, że używasz Archa (btw)."*  
> *"Because your regular fetch wasn't bloated enough, and everyone must know you use Arch btw."*

**FATfetch** to satyryczny, potężny i superszybki odpowiednik narzędzi typu `neofetch`/`fastfetch` napisany w **C++20** z dedykacją dla społeczności **Arch Linux**. Zamiast nudnych statystyk, FATfetch serwuje bezlitosne roasty o niebraniu prysznica, 500 godzinach spędzonych na animacjach w Hyprlandzie, braku dziewczyny i diecie opartej na Monsterkach z AUR.

---

## 📸 Funkcje / Features

- 🦣 **Arch T-Shirt Big Guy w ASCII**: Klasyczny, kultowy mem z potężnym gościem w koszulce Arch Linux w wielokolorowym ANSI art.
- 🌐 **Pełne wsparcie dla Locales (Polski 🇵🇱 / English 🇺🇸)**:
  - Automatyczne wykrywanie języka systemu (`$LANG` / `$LC_ALL`).
  - Możliwość wymuszenia języka: `fatfetch --lang pl` lub `fatfetch --lang en`.
  - Osobne, dedykowane bazy żartów, roastów, etykiet i planów dietetycznych dla każdego języka.
- ⚡ **Natywny C++20**: Zero bloatu w kodzie (maksimum bloatu w statystykach). Działa błyskawicznie bez zbędnych zależności.
- 🧠 **Generator żartów i roastów o Archu**: Za każdym odpaleniem dostajesz nową dawkę humoru o użytkownikach Archa, `pacman -Syu`, Neovimie i zakolanówkach.
- 🖥️ **Prawdziwe dane z systemu z memicznym twistem**:
  - Pakiety: *979 (pacman/AUR), 2 (flatpakowy bloat)*
  - Uptime: *420d 69m (zero restartów, zero prysznica)*
  - Pamięć: *94% zjedzone przez Discorda i 50 kart Arch Wiki*
  - Grass / Trawa: *0.00 ms (Rekord życiowy: trawnik nie wspiera Waylanda)*
  - Dziewczyna: *Błąd segmentacji (zrzut pamięci) - Brak w repozytoriach*
- 🛠️ **Interaktywny instalator TUI (`fatfetch-installer` / `./install.sh`)**: Z wyborem języka, animacją kompilacji, wyborem ścieżki instalacji i opcją dodania do `.bashrc`/`.zshrc`.
- 📦 **Wsparcie dla PKGBUILD**: Możliwość budowania przez `makepkg -si`.

---

## 🚀 Szybka instalacja / Quick Install

### Opcja 1: Interaktywny instalator TUI (Zalecane / Recommended)
```bash
./install.sh
```
*Instalator zapyta o język (Polski / English), wyświetli potężnego gościa w ASCII i pozwoli zainstalować program do `~/.local/bin/fatfetch` lub `/usr/local/bin/fatfetch`.*

### Opcja 2: Kompilacja przez `make`
```bash
make
make install
```

### Opcja 3: Arch Linux `PKGBUILD`
```bash
makepkg -si
```

---

## 🕹️ Użycie i Flagi / Usage & Flags

```bash
# Standardowy fetch (automatycznie dopasowuje język systemu)
fatfetch

# Wymuszenie języka polskiego lub angielskiego
fatfetch --lang pl
fatfetch --lang en

# Wybierz inne logo ASCII (archguy, fatarch, discordmod, minimal)
fatfetch --logo fatarch
fatfetch --logo discordmod

# Wylosuj tylko świeży żart o Archu (po polsku lub angielsku)
fatfetch -j
fatfetch --lang en --joke

# Wyświetl oficjalną dietę 4500 kcal użytkownika Archa
fatfetch --diet
fatfetch --lang en --diet

# Tryb surowy (bez kolorów ANSI)
fatfetch --raw

# Pomoc i wersja
fatfetch --help
fatfetch --version
```

---

## 🖼️ Dostępne presety ASCII Logo

1. `archguy` *(domyślne)* - Legendarny gość w koszulce Arch Linux.
2. `fatarch` - Ultra-szerokie, spasiony logo Archa `/\`.
3. `discordmod` - Mod z piwnicy.
4. `minimal` - Kompaktowe małe logo dla małych okienek terminala.

---

## 📜 Licencja

MIT - Używaj, forku i pamiętaj: *BTW, I use Arch.*
