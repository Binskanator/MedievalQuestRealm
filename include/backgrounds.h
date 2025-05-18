#ifndef BACKGROUNDS_H
#define BACKGROUNDS_H

#include "gba.h"

// Background types
typedef enum {
    BG_TITLE,
    BG_FIELD,
    BG_TOWN,
    BG_CASTLE,
    BG_DUNGEON,
    BG_BATTLE,
    BG_SHOP,
    BG_TRAINING
} BackgroundType;

// Initialize background system
void initBackgrounds();

// Load a background
void loadBackground(BackgroundType type, int bgIndex);

// Scroll a background
void scrollBackground(int bgIndex, int x, int y);

// Set background position
void setBackgroundPosition(int bgIndex, int x, int y);

// Update animated backgrounds
void updateBackgrounds();

// Get tile in a background
unsigned short getBackgroundTile(int bgIndex, int x, int y);

// Set tile in a background
void setBackgroundTile(int bgIndex, int x, int y, unsigned short tileId);

// Load tile data for backgrounds
void loadBackgroundTiles(int bgIndex, const unsigned short* tiles, int count);

// Load tile map for backgrounds
void loadBackgroundMap(int bgIndex, const unsigned short* map, int width, int height);

// Load palette for backgrounds
void loadBackgroundPalette(const unsigned short* palette, int count);

#endif // BACKGROUNDS_H
