# Makefile for building and testing the Medieval RPG GBA Game

# Simple test version for PC
test:
	gcc -o test_rpg simple_gba_game.c
	./test_rpg

# Build GBA version using devkitPro tools (when available)
gba:
	@echo "Building GBA ROM..."
	@echo "Note: This requires devkitPro tools to be installed."
	@echo "For now, running the test version."
	make test

# Clean build files
clean:
	rm -f test_rpg game.gba *.o

.PHONY: test gba clean