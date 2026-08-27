CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wall -Wextra -static-libstdc++ -static-libgcc -Isrc
PREFIX ?= $(if $(filter root,$(USER)),/usr/local,$(HOME)/.local)
DIST_ZIP = FATfetch-standalone.zip

SRC = src/main.cpp src/sysinfo.cpp src/jokes.cpp src/ascii_art.cpp src/display.cpp src/locales.cpp src/palettes.cpp src/config.cpp src/tui_config.cpp
INSTALLER_SRC = src/installer/installer.cpp src/locales.cpp src/config.cpp src/tui_config.cpp src/palettes.cpp src/ascii_art.cpp src/display.cpp src/sysinfo.cpp src/jokes.cpp

all: fatfetch fatfetch-installer

fatfetch: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o fatfetch

fatfetch-installer: $(INSTALLER_SRC)
	$(CXX) $(CXXFLAGS) $(INSTALLER_SRC) -o fatfetch-installer

install: fatfetch
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f fatfetch $(DESTDIR)$(PREFIX)/bin/fatfetch
	chmod 755 $(DESTDIR)$(PREFIX)/bin/fatfetch
	@echo "FATfetch installed to $(DESTDIR)$(PREFIX)/bin/fatfetch"

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/fatfetch
	@echo "FATfetch uninstalled from $(DESTDIR)$(PREFIX)/bin/fatfetch"

zip: all
	chmod +x install.sh fatfetch fatfetch-installer
	zip -r $(DIST_ZIP) fatfetch fatfetch-installer install.sh README.md Makefile PKGBUILD LICENSE src/
	@echo "Created $(DIST_ZIP) successfully!"

clean:
	rm -f fatfetch fatfetch-installer $(DIST_ZIP)

.PHONY: all install uninstall clean zip
