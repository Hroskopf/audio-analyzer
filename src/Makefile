OS := $(shell uname -s)

ifeq ($(OS), Darwin) # for MacOS
    INCLUDE_DIRS = -I$(shell brew --prefix)/include
    LIB_DIRS = -L$(shell brew --prefix)/lib
    LIBS = -lfftw3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    INSTALL_CMD = brew install fftw sfml
    UNINSTALL_CMD = brew uninstall fftw sfml
else ifeq ($(OS), Linux) # for Linux
    INCLUDE_DIRS = -I/usr/include
    LIB_DIRS = -L/usr/lib
    LIBS = -lfftw3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
    INSTALL_CMD = \
        if command -v apt >/dev/null; then \
            echo "Using apt"; \
            sudo apt update && sudo apt install -y libsfml-dev libfftw3-dev; \
        elif command -v dnf >/dev/null; then \
            echo "Using dnf"; \
            sudo dnf install -y SFML-devel fftw-devel; \
        elif command -v pacman >/dev/null; then \
            echo "Using pacman"; \
            sudo pacman -Sy --noconfirm sfml fftw; \
        else \
            echo "Unsupported package manager"; \
            exit 1; \
        fi
    
    UNINSTALL_CMD = \
        if command -v apt >/dev/null; then \
            echo "Using apt"; \
            sudo apt remove -y libsfml-dev libfftw3-dev; \
        elif command -v dnf >/dev/null; then \
            echo "Using dnf"; \
            sudo dnf remove -y SFML-devel fftw-devel; \
        elif command -v pacman >/dev/null; then \
            echo "Using pacman"; \
            sudo pacman -Rns --noconfirm sfml fftw; \
        else \
            echo "Unsupported package manager"; \
            exit 1; \
        fi
endif

audioanalyzer: main.cpp
	g++ -std=c++17 -o $@ $^ $(INCLUDE_DIRS) $(LIB_DIRS) $(LIBS)

install-libraries:
    
	$(INSTALL_CMD)

uninstall-libraries:
	$(UNINSTALL_CMD)

clean:
	rm -f audioanalyzer

.PHONY: clean install-libraries uninstall-libraries
