CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -static-libstdc++ -static-libgcc -Isrc
PREFIX ?= $(if $(filter root,$(USER)),/usr/local,$(HOME)/.local)
DIST_ZIP = FATfetch-standalone.zip

COMMON_SRC = src/sysinfo.cpp src/jokes.cpp src/ascii_art.cpp src/display.cpp src/locales.cpp src/palettes.cpp src/config.cpp src/tui_config.cpp src/fatjump.cpp src/discord_rpc.cpp src/fatgotchi.cpp src/fatrain.cpp src/fatfight.cpp
SRC = src/main.cpp $(COMMON_SRC)
FATJUMP_SRC = src/fatjump_main.cpp $(COMMON_SRC)
FATRPC_SRC = src/fatrpc_main.cpp $(COMMON_SRC)
FATGOTCHI_SRC = src/fatgotchi_main.cpp $(COMMON_SRC)
FATRAIN_SRC = src/fatrain_main.cpp $(COMMON_SRC)
FATFIGHT_SRC = src/fatfight_main.cpp $(COMMON_SRC)
INSTALLER_SRC = src/installer/installer.cpp $(COMMON_SRC)

all: fatfetch fatjump fatrpc fatgotchi fatrain fatfight fatfetch-installer

fatfetch: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o fatfetch

fatjump: $(FATJUMP_SRC)
	$(CXX) $(CXXFLAGS) $(FATJUMP_SRC) -o fatjump

fatrpc: $(FATRPC_SRC)
	$(CXX) $(CXXFLAGS) $(FATRPC_SRC) -o fatrpc

fatgotchi: $(FATGOTCHI_SRC)
	$(CXX) $(CXXFLAGS) $(FATGOTCHI_SRC) -o fatgotchi

fatrain: $(FATRAIN_SRC)
	$(CXX) $(CXXFLAGS) $(FATRAIN_SRC) -o fatrain

fatfight: $(FATFIGHT_SRC)
	$(CXX) $(CXXFLAGS) $(FATFIGHT_SRC) -o fatfight

fatfetch-installer: $(INSTALLER_SRC)
	$(CXX) $(CXXFLAGS) $(INSTALLER_SRC) -o fatfetch-installer

install: fatfetch fatjump fatrpc fatgotchi fatrain fatfight
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f fatfetch $(DESTDIR)$(PREFIX)/bin/fatfetch
	cp -f fatjump $(DESTDIR)$(PREFIX)/bin/fatjump
	cp -f fatrpc $(DESTDIR)$(PREFIX)/bin/fatrpc
	cp -f fatgotchi $(DESTDIR)$(PREFIX)/bin/fatgotchi
	cp -f fatrain $(DESTDIR)$(PREFIX)/bin/fatrain
	cp -f fatfight $(DESTDIR)$(PREFIX)/bin/fatfight
	chmod 755 $(DESTDIR)$(PREFIX)/bin/fatfetch $(DESTDIR)$(PREFIX)/bin/fatjump $(DESTDIR)$(PREFIX)/bin/fatrpc $(DESTDIR)$(PREFIX)/bin/fatgotchi $(DESTDIR)$(PREFIX)/bin/fatrain $(DESTDIR)$(PREFIX)/bin/fatfight
	@echo "FATfetch suite installed to $(DESTDIR)$(PREFIX)/bin"

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/fatfetch $(DESTDIR)$(PREFIX)/bin/fatjump $(DESTDIR)$(PREFIX)/bin/fatrpc $(DESTDIR)$(PREFIX)/bin/fatgotchi $(DESTDIR)$(PREFIX)/bin/fatrain $(DESTDIR)$(PREFIX)/bin/fatfight
	@echo "Uninstalled from $(DESTDIR)$(PREFIX)/bin"

zip: all
	chmod +x install.sh fatfetch fatjump fatrpc fatgotchi fatrain fatfight fatfetch-installer
	zip -r $(DIST_ZIP) fatfetch fatjump fatrpc fatgotchi fatrain fatfight fatfetch-installer install.sh README.md Makefile PKGBUILD LICENSE src/
	@echo "Created $(DIST_ZIP) successfully!"

clean:
	rm -f fatfetch fatjump fatrpc fatgotchi fatrain fatfight fatfetch-installer $(DIST_ZIP)

.PHONY: all install uninstall clean zip
