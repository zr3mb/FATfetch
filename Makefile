CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -static-libstdc++ -static-libgcc -Isrc
PREFIX ?= $(if $(filter root,$(USER)),/usr/local,$(HOME)/.local)
DIST_ZIP = FATfetch-standalone.zip

COMMON_SRC = src/sysinfo.cpp src/jokes.cpp src/ascii_art.cpp src/display.cpp src/locales.cpp src/palettes.cpp src/config.cpp src/tui_config.cpp src/fatjump.cpp
SRC = src/main.cpp $(COMMON_SRC)
FATJUMP_SRC = src/fatjump_main.cpp $(COMMON_SRC)
INSTALLER_SRC = src/installer/installer.cpp src/locales.cpp src/config.cpp src/tui_config.cpp src/palettes.cpp src/ascii_art.cpp src/display.cpp src/sysinfo.cpp src/jokes.cpp src/fatjump.cpp

all: fatfetch fatjump fatfetch-installer

fatfetch: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o fatfetch

fatjump: $(FATJUMP_SRC)
	$(CXX) $(CXXFLAGS) $(FATJUMP_SRC) -o fatjump

fatfetch-installer: $(INSTALLER_SRC)
	$(CXX) $(CXXFLAGS) $(INSTALLER_SRC) -o fatfetch-installer

install: fatfetch fatjump
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f fatfetch $(DESTDIR)$(PREFIX)/bin/fatfetch
	cp -f fatjump $(DESTDIR)$(PREFIX)/bin/fatjump
	chmod 755 $(DESTDIR)$(PREFIX)/bin/fatfetch $(DESTDIR)$(PREFIX)/bin/fatjump
	@echo "FATfetch and FATjump installed to $(DESTDIR)$(PREFIX)/bin"

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/fatfetch $(DESTDIR)$(PREFIX)/bin/fatjump
	@echo "FATfetch and FATjump uninstalled from $(DESTDIR)$(PREFIX)/bin"

zip: all
	chmod +x install.sh fatfetch fatjump fatfetch-installer
	zip -r $(DIST_ZIP) fatfetch fatjump fatfetch-installer install.sh README.md Makefile PKGBUILD LICENSE src/
	@echo "Created $(DIST_ZIP) successfully!"

clean:
	rm -f fatfetch fatjump fatfetch-installer $(DIST_ZIP)

.PHONY: all install uninstall clean zip
