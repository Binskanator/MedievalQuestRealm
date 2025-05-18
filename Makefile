
# Main Makefile for Medieval RPG GBA Game

# Default target - build the GBA version
all: gba

# Build GBA version using devkitPro tools
gba:
	@$(MAKE) -f Makefile.gba

# Simple test version for PC (for development/testing)
test:
	gcc -o test_rpg simple_gba_game.c
	./test_rpg

# Clean build files
clean:
	rm -f test_rpg *.o
	$(MAKE) -f Makefile.gba clean

.PHONY: all gba test clean
