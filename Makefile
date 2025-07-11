# Makefile for Simple SDL 3 Window Application

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99

# SDL3 flags (using pkg-config)
SDL_CFLAGS = $(shell pkg-config --cflags sdl3)
SDL_LIBS = $(shell pkg-config --libs sdl3)

# Target executable
TARGET = sdl3_window

# Source files
SOURCES = main.cpp

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -o $(TARGET) $(SOURCES) $(SDL_LIBS)

# Clean build files
clean:
	rm -f $(TARGET)

# Run the application
run: $(TARGET)
	./$(TARGET)

# Install SDL3 dependencies (Ubuntu/Debian)
install-deps:
	sudo apt update
	sudo apt install libsdl3-dev pkg-config build-essential

# Install SDL3 dependencies (Fedora/RHEL)
install-deps-fedora:
	sudo dnf install SDL3-devel pkgconfig gcc

# Install SDL3 dependencies (Arch Linux)
install-deps-arch:
	sudo pacman -S sdl3 pkgconfig gcc

.PHONY: all clean run install-deps install-deps-fedora install-deps-arch
