#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "gba.h"

// Graphics modes
typedef enum {
    GRAPHICS_MODE_TILE,
    GRAPHICS_MODE_BITMAP
} GraphicsMode;

// Sprite flip modes
typedef enum {
    FLIP_NONE = 0,
    FLIP_HORIZONTAL = 1,
    FLIP_VERTICAL = 2,
    FLIP_BOTH = 3
} FlipMode;

// Initialize the graphics system
void initGraphics(GraphicsMode mode);

// Wait for vertical blank period
void waitForVBlank();

// Set a pixel in bitmap mode
void setPixel(int x, int y, Color color);

// Draw a rectangle
void drawRect(int x, int y, int width, int height, Color color);

// Draw a hollow rectangle (outline)
void drawHollowRect(int x, int y, int width, int height, Color color);

// Draw a line
void drawLine(int x1, int y1, int x2, int y2, Color color);

// Fill the screen with a color
void fillScreen(Color color);

// Draw an image from a byte array
void drawImage(int x, int y, int width, int height, const unsigned short* image);

// Draw a sprite using OAM (Object Attribute Memory)
void drawSprite(int id, int x, int y, int tileIndex, int palBank, FlipMode flip);

// Hide a sprite
void hideSprite(int id);

// Update all sprites
void updateSprites();

// Load palette data
void loadPalette(const unsigned short* palette, int startIndex, int size);

// Load tile data
void loadTiles(const unsigned short* tiles, int startIndex, int size);

// Print text on screen
void drawText(int x, int y, const char* text, Color color);

#endif // GRAPHICS_H
