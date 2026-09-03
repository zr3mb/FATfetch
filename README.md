# 🍔 FATfetch - The Heaviest System Fetch for Arch Linux (C++)

[![Arch Linux](https://img.shields.io/badge/Arch_Linux-btw_i_use_arch-1793d1?logo=arch-linux)](https://archlinux.org/)
[![Bloat Level](https://img.shields.io/badge/Bloat-OVER_9000-ff0055)](#)
[![Hygiene](https://img.shields.io/badge/Shower-0%20Days-critical)](#)
[![Grass Status](https://img.shields.io/badge/Grass-Untouched-red)](#)
[![Femboy Mode](https://img.shields.io/badge/Femboy-300kg%20UwU-ff69b4)](#)
[![Hyfetch Palettes](https://img.shields.io/badge/Palettes-10%20Themes-9c59d1)](#)

> *"Bo Twój zwykły fastfetch/neofetch nie był wystarczająco spasiony, a świat musi wiedzieć, że używasz Archa (btw)."*  
> *"Because your regular fetch wasn't bloated enough, and everyone must know you use Arch btw."*

**FATfetch** to satyryczny, potężny i superszybki odpowiednik narzędzi typu `neofetch`/`fastfetch`/`hyfetch` napisany w **C++20** z dedykacją dla społeczności **Arch Linux**.

---

## 📸 Nowe Funkcje / Features

- 🌸 **Gruby Femboy w ASCII (`fatfemboy`)**: 300kg UwU w zakolanówkach programistycznych (programming socks) i za dużej bluzie z logo Arch Linux.
- 🎨 **Palety kolorów niczym w Hyfetchu (Pride / LGBT / Aesthetics)**:
  - `femboy` (Pastel Pink / White / Cyan)
  - `trans` (Transgender Pride Flag)
  - `rainbow` / `pride` (Classic Rainbow Pride Flag)
  - `bi` (Bisexual Pride Flag)
  - `pan` (Pansexual Pride Flag)
  - `nonbinary` (Non-Binary Pride Flag)
  - `lesbian` (Lesbian Pride Flag)
  - `catppuccin` (Catppuccin Mocha Palette)
  - `dracula` (Cyberpunk Dracula Palette)
  - `default` (Arch Classic Cyan)
- 🎛️ **Graficzny konfigurator TUI (`fatfetch --config`)**:
  - Interaktywne menu w terminalu z **podglądem na żywo (Live Preview)** logo ASCII oraz pasków kolorów.
  - Zapisywanie konfiguracji w `~/.config/fatfetch/config.conf`.
- 🦣 **Arch T-Shirt Big Guy w ASCII (`archguy`)**: Kultowy mem z potężnym gościem w koszulce Arch Linux w wielokolorowym ANSI art.
- 🌐 **Wielojęzyczność (Polski 🇵🇱 / English 🇺🇸)**: Osobne bazy żartów, etykiet i diet.
- ⚡ **C++20 ze statycznym linkowaniem**: Błyskawiczny czas uruchomienia, 0 zależności.

---

## 🚀 Szybka instalacja / Quick Install

```bash
git clone https://github.com/zr3mb/FATfetch.git
cd FATfetch
./install.sh
```

---

## 🕹️ Konfigurator TUI & Gry i Komendy

### 🎮 Nowa Gra: `fatgotchi` (Tamagotchi z Arch Chadem):
Karm grubasa burgerami 6XL i pierogami oraz nawadniaj Monsterem Ultra White w czasie rzeczywistym! Uważaj: jeśli schudnie – przegrywasz, ale jeśli przekarmisz burgerami – **bebech pęknie i zaleje klawiaturę sosem czosnkowym**!
```bash
fatgotchi
# lub: fatfetch --game
```
**Sterowanie w grze:**
- `[1]` / `[b]` – 🍔 Karm Burgerem 6XL (+12% Tłuszczu)
- `[2]` / `[m]` – 🥫 Puszka Monstera Ultra White (+15% Nawodnienia)
- `[3]` / `[p]` – 🥟 Talerz Pierogów ze skwarkami (+20% Tłuszczu)
- `[4]` / `[k]` – 💻 Kompiluj jądro i Chromium z AUR (+150 pkt, -14% Tłuszczu)
- `[q]` – Wyjście z gry

---

### 🎮 Discord Rich Presence (`fatrpc` / `fatfetch --rpc`):
Wyświetla na Twoim profilu Discordzie status z wybraną postacią (Arch Chad, Gruby Femboy, Discord Mod) oraz statystykami:
```bash
# Włącz autostart demona w tle (systemd / XDG autostart):
fatrpc enable
# lub: fatfetch --rpc-enable

# Wyłącz autostart:
fatrpc disable

# Sprawdź status połączenia z Discordem i demona:
fatrpc status

# Odpal w trybie podglądu na żywo w bieżącym terminalu:
fatrpc
# lub: fatfetch --rpc
```

---

### 🚀 Nowy Widżet: `fatjump` (Skaczący Grubas w Slow-Mo):
Odpala w pętli ultra-płynną animację w zwolnionym tempie ze skaczącym grubasem, fizyką falującego bebecha, latającymi puszkami Monstera, chrupkami i sejsmografem Richtera:
```bash
fatjump
# lub z poziomu fatfetcha:
fatfetch --jump
```
**Sterowanie w `fatjump`:**
- `[Spacja]` – Zmiana prędkości: Matrix Ultra Slow-Mo (240ms) / Klasyczny Slow-Mo (130ms) / Turbo Jiggle (60ms)
- `[p]` – Przełączanie palet kolorystycznych w locie (Femboy, Trans, Pride, Dracula, Catppuccin, Cyan)
- `[q]` / `[Esc]` – Wyjście

---

### Uruchomienie graficznego konfiguratora TUI:
```bash
fatfetch --config
# lub
fatfetch -c
```

### Przykłady wywołania z flagami:
```bash
# Uruchomienie z grubym femboyem i pastelową paletą Femboy
fatfetch --logo fatfemboy --palette femboy

# Paleta Trans Pride
fatfetch --palette trans

# Paleta Rainbow / Pride
fatfetch --palette rainbow

# Motyw Catppuccin Mocha
fatfetch --palette catppuccin

# Lista wszystkich dostępnych palet z kolorowymi podglądami
fatfetch --list-palettes

# Lista dostępnych postaci ASCII
fatfetch --list-logos

# Losowy żart o Archu
fatfetch --joke
```

---

## ⚙️ Plik konfiguracyjny (`~/.config/fatfetch/config.conf`)

Konfigurację możesz edytować przez TUI (`fatfetch --config`) lub ręcznie w pliku:

```ini
# Logo preset: archguy, fatfemboy, fatarch, discordmod, minimal
logo = fatfemboy

# Color palette: default, femboy, trans, rainbow, bi, pan, nonbinary, lesbian, catppuccin, dracula
palette = femboy

# Language: pl, en
lang = pl

# Show randomized Arch Linux meme joke on run
show_joke = true

# Show bottom color palette blocks
color_blocks = true
```

---

## 📜 Licencja

MIT - *BTW, I use Arch.*
