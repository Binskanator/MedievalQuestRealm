#include "../include/graphics.h"
#include "../include/text.h"
#include <stdlib.h> // Required for abs()
#include "../include/globals.h"
// Current graphics mode
GraphicsMode currentMode;

// OAM buffer for sprites
OBJ_ATTR shadowOAM[128];

// Initialize the graphics system
void initGraphics(GraphicsMode mode) {
    currentMode = mode;
    
    if (mode == GRAPHICS_MODE_TILE) {
        REG_DISPCNT = MODE_0 | BG0_ENABLE | BG1_ENABLE | OBJ_ENABLE | OBJ_MAP_1D;
    } else {
        REG_DISPCNT = MODE_3 | BG2_ENABLE;
        videoBuffer = (unsigned short*)0x6000000;
    }

    // Clear OAM
    for (int i = 0; i < 128; i++) {
        shadowOAM[i].attr0 = ATTR0_HIDE;
    }
}

// Wait for vertical blank period
void waitForVBlank() {
    while (REG_DISPSTAT & VBLANK_FLAG);
    while (!(REG_DISPSTAT & VBLANK_FLAG));
}

// Set a pixel in bitmap mode
void setPixel(int x, int y, Color color) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        videoBuffer[y * SCREEN_WIDTH + x] = color;
    }
}

// Draw a filled rectangle
void drawRect(int x, int y, int width, int height, Color color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            setPixel(x + j, y + i, color);
        }
    }
}

// Draw a hollow rectangle
void drawHollowRect(int x, int y, int width, int height, Color color) {
    for (int i = 0; i < width; i++) {
        setPixel(x + i, y, color);
        setPixel(x + i, y + height - 1, color);
    }

    for (int i = 0; i < height; i++) {
        setPixel(x, y + i, color);
        setPixel(x + width - 1, y + i, color);
    }
}

// Draw a line using Bresenham's algorithm
void drawLine(int x1, int y1, int x2, int y2, Color color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        setPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Fill the screen with a color
void fillScreen(Color color) {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            setPixel(j, i, color);
        }
    }
}

// Draw an image
void drawImage(int x, int y, int width, int height, const unsigned short* image) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            setPixel(x + j, y + i, image[i * width + j]);
        }
    }
}

// Draw a sprite using OAM
void drawSprite(int id, int x, int y, int tileIndex, int palBank, FlipMode flip) {
    if (id >= 0 && id < 128) {
        shadowOAM[id].attr0 = (y & 0xFF) | ATTR0_SQUARE;
        shadowOAM[id].attr1 = (x & 0x1FF) |
                              ((flip == FLIP_HORIZONTAL || flip == FLIP_BOTH) ? ATTR1_FLIP_X : 0) |
                              ((flip == FLIP_VERTICAL || flip == FLIP_BOTH) ? ATTR1_FLIP_Y : 0) |
                              ATTR1_SIZE_16;
        shadowOAM[id].attr2 = tileIndex | ATTR2_PALROW(palBank);
    }
}

// Hide a sprite
void hideGraphicsSprite(int id) {
    if (id >= 0 && id < 128) {
        shadowOAM[id].attr0 = ATTR0_HIDE;
    }
}

// Update all sprites
void updateSprites() {
    for (int i = 0; i < 128; i++) {
        OAM[i] = shadowOAM[i];
    }
}

// Load palette data
void loadPalette(const unsigned short* palette, int startIndex, int size) {
    volatile unsigned short* paletteMemory = (volatile unsigned short*)PALETTE_BASE;
    for (int i = 0; i < size; i++) {
        paletteMemory[startIndex + i] = palette[i];
    }
}

// Load tile data
void loadTiles(const unsigned short* tiles, int startIndex, int size) {
    volatile unsigned short* tileMemory = (volatile unsigned short*)TILE_BASE_ADDR(0);
    for (int i = 0; i < size; i++) {
        tileMemory[startIndex + i] = tiles[i];
    }
}
