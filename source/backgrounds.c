#include "../include/backgrounds.h"
#include <string.h>

// Background map data (would normally be stored in separate binary files)
// These are minimal placeholder maps for demonstration purposes
unsigned short titleMapData[32*32] = {0};
unsigned short fieldMapData[32*32] = {0};
unsigned short townMapData[32*32] = {0};
unsigned short castleMapData[32*32] = {0};
unsigned short dungeonMapData[32*32] = {0};
unsigned short battleMapData[32*32] = {0};
unsigned short shopMapData[32*32] = {0};
unsigned short trainingMapData[32*32] = {0};

// Tile data for different background types (would normally be stored in separate binary files)
unsigned short bgTiles[4096] = {0}; // Space for up to 256 tiles (16 bytes per tile)

// Current background offsets
int bgOffsets[4][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}}; // [bg_index][x/y]

// Animation frame counters for animated backgrounds
int bgAnimFrames[4] = {0, 0, 0, 0};

// Initialize background system
void initBackgrounds() {
    // Set up background control registers for each background layer
    REG_BG0CNT = 0x1080; // Priority 0, 8bpp tiles, use 2nd tile block, use 8th map block
    REG_BG1CNT = 0x1181; // Priority 1, 8bpp tiles, use 2nd tile block, use 9th map block
    REG_BG2CNT = 0x1282; // Priority 2, 8bpp tiles, use 2nd tile block, use 10th map block
    REG_BG3CNT = 0x1383; // Priority 3, 8bpp tiles, use 2nd tile block, use 11th map block
    
    // Create default tile data - just a few basic tiles for the game to use
    // Tile 0: Empty (transparent)
    memset(&bgTiles[0], 0, 16);
    
    // Tile 1: Solid block
    for (int i = 0; i < 16; i++) {
        bgTiles[i + 16] = 0xFFFF;
    }
    
    // Tile 2: Checkerboard
    for (int i = 0; i < 16; i++) {
        bgTiles[i + 32] = (i % 2) ? 0xF0F0 : 0x0F0F;
    }
    
    // Fill the various maps with placeholder patterns
    // These would normally be loaded from data files
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            // Title screen - simple border
            if (x == 0 || y == 0 || x == 31 || y == 31) {
                titleMapData[y * 32 + x] = 1;
            } else {
                titleMapData[y * 32 + x] = 0;
            }
            
            // Field - mix of empty and grass tiles
            fieldMapData[y * 32 + x] = (((x + y) % 5) == 0) ? 2 : ((x + y) % 3);
            
            // Town - more structured pattern
            if ((x % 8 < 6) && (y % 8 < 6)) {
                townMapData[y * 32 + x] = (x % 3) + (y % 2);
            } else {
                townMapData[y * 32 + x] = 1; // Roads
            }
            
            // Castle - regular pattern
            castleMapData[y * 32 + x] = ((x % 4 == 0) || (y % 4 == 0)) ? 1 : 0;
            
            // Dungeon - maze-like
            dungeonMapData[y * 32 + x] = ((x % 3 == 0) || (y % 3 == 0)) ? 1 : ((x + y) % 2);
            
            // Battle - simple arena
            if (y < 8 || y > 24 || x < 8 || x > 24) {
                battleMapData[y * 32 + x] = 1;
            } else {
                battleMapData[y * 32 + x] = 0;
            }
            
            // Shop - room with counter
            if (y == 15 && x > 8 && x < 24) {
                shopMapData[y * 32 + x] = 1; // Counter
            } else if (y < 5 || y > 25 || x < 5 || x > 25) {
                shopMapData[y * 32 + x] = 1; // Walls
            } else {
                shopMapData[y * 32 + x] = 0; // Floor
            }
            
            // Training - training ground
            if ((x % 10 < 8) && (y % 10 < 8)) {
                trainingMapData[y * 32 + x] = (x + y) % 2;
            } else {
                trainingMapData[y * 32 + x] = 2; // Special training tile
            }
        }
    }
    
    // Load the tiles into VRAM
    loadBackgroundTiles(0, bgTiles, 256);
}

// Load a background
void loadBackground(BackgroundType type, int bgIndex) {
    unsigned short* mapData = NULL;
    
    // Select the appropriate map data
    switch (type) {
        case BG_TITLE:
            mapData = titleMapData;
            break;
        case BG_FIELD:
            mapData = fieldMapData;
            break;
        case BG_TOWN:
            mapData = townMapData;
            break;
        case BG_CASTLE:
            mapData = castleMapData;
            break;
        case BG_DUNGEON:
            mapData = dungeonMapData;
            break;
        case BG_BATTLE:
            mapData = battleMapData;
            break;
        case BG_SHOP:
            mapData = shopMapData;
            break;
        case BG_TRAINING:
            mapData = trainingMapData;
            break;
        default:
            // Default to empty map
            mapData = titleMapData;
            break;
    }
    
    // Load the map data
    loadBackgroundMap(bgIndex, mapData, 32, 32);
    
    // Reset the scroll position
    setBackgroundPosition(bgIndex, 0, 0);
    
    // Reset animation frame
    bgAnimFrames[bgIndex] = 0;
}

// Scroll a background
void scrollBackground(int bgIndex, int x, int y) {
    bgOffsets[bgIndex][0] = (bgOffsets[bgIndex][0] + x) & 0x1FF;
    bgOffsets[bgIndex][1] = (bgOffsets[bgIndex][1] + y) & 0x1FF;
    
    // Update the hardware registers
    switch (bgIndex) {
        case 0:
            REG_BG0HOFS = bgOffsets[0][0];
            REG_BG0VOFS = bgOffsets[0][1];
            break;
        case 1:
            REG_BG1HOFS = bgOffsets[1][0];
            REG_BG1VOFS = bgOffsets[1][1];
            break;
        case 2:
            REG_BG2HOFS = bgOffsets[2][0];
            REG_BG2VOFS = bgOffsets[2][1];
            break;
        case 3:
            REG_BG3HOFS = bgOffsets[3][0];
            REG_BG3VOFS = bgOffsets[3][1];
            break;
    }
}

// Set background position
void setBackgroundPosition(int bgIndex, int x, int y) {
    bgOffsets[bgIndex][0] = x & 0x1FF;
    bgOffsets[bgIndex][1] = y & 0x1FF;
    
    // Update the hardware registers
    switch (bgIndex) {
        case 0:
            REG_BG0HOFS = bgOffsets[0][0];
            REG_BG0VOFS = bgOffsets[0][1];
            break;
        case 1:
            REG_BG1HOFS = bgOffsets[1][0];
            REG_BG1VOFS = bgOffsets[1][1];
            break;
        case 2:
            REG_BG2HOFS = bgOffsets[2][0];
            REG_BG2VOFS = bgOffsets[2][1];
            break;
        case 3:
            REG_BG3HOFS = bgOffsets[3][0];
            REG_BG3VOFS = bgOffsets[3][1];
            break;
    }
}

// Update animated backgrounds
void updateBackgrounds() {
    // Increment animation frames
    for (int i = 0; i < 4; i++) {
        bgAnimFrames[i] = (bgAnimFrames[i] + 1) % 60;
    }
    
    // Example of an animated background effect (water shimmer, etc.)
    // In a real game, this would update specific tiles or scroll the background
    
    // For now, we'll just demonstrate a simple scrolling effect on BG1 if it's active
    if (REG_DISPCNT & BG1_ENABLE) {
        // Gentle scrolling for water/clouds effect
        scrollBackground(1, 1, 0);
    }
}

// Get tile in a background
unsigned short getBackgroundTile(int bgIndex, int x, int y) {
    unsigned short* mapPtr = NULL;
    
    // Get the correct map data
    switch (bgIndex) {
        case 0:
            mapPtr = (unsigned short*)MAP_BASE_ADDR(8);
            break;
        case 1:
            mapPtr = (unsigned short*)MAP_BASE_ADDR(9);
            break;
        case 2:
            mapPtr = (unsigned short*)MAP_BASE_ADDR(10);
            break;
        case 3:
            mapPtr = (unsigned short*)MAP_BASE_ADDR(11);
            break;
    }
    
    if (mapPtr && x >= 0 && x < 32 && y >= 0 && y < 32) {
        return mapPtr[y * 32 + x];
    }
    
    return 0;
}

// Set tile in a background
void setBackgroundTile(int bgIndex, int x, int y, unsigned short tileId) {
    volatile unsigned short* mapPtr = NULL;
    
    // Get the correct map data
    switch (bgIndex) {
        case 0:
            mapPtr = (volatile unsigned short*)MAP_BASE_ADDR(8);
            break;
        case 1:
            mapPtr = (volatile unsigned short*)MAP_BASE_ADDR(9);
            break;
        case 2:
            mapPtr = (volatile unsigned short*)MAP_BASE_ADDR(10);
            break;
        case 3:
            mapPtr = (volatile unsigned short*)MAP_BASE_ADDR(11);
            break;
    }
    
    if (mapPtr && x >= 0 && x < 32 && y >= 0 && y < 32) {
        mapPtr[y * 32 + x] = tileId;
    }
}

// Load tile data for backgrounds
void loadBackgroundTiles(int bgIndex, const unsigned short* tiles, int count) {
    volatile unsigned short* tileMemory = (volatile unsigned short*)TILE_BASE_ADDR(2);
    
    // Load tiles into the appropriate area of VRAM
    for (int i = 0; i < count * 8; i++) {  // 8 shorts per tile
        tileMemory[i] = tiles[i];
    }
}

// Load tile map for backgrounds
void loadBackgroundMap(int bgIndex, const unsigned short* map, int width, int height) {
    volatile unsigned short* mapPtr = NULL;
    
    // Get the correct map location
    switch (bgIndex) {
        case 0:
            mapPtr = (volatile unsigned short*)MAP_BASE_ADDR(8);
            break;
        case 1:
            mapPtr = (volatile unsigned short*)MAP_BASE_ADDR(9);
            break;
        case 2:
            mapPtr = (volatile unsigned short*)MAP_BASE_ADDR(10);
            break;
        case 3:
            mapPtr = (volatile unsigned short*)MAP_BASE_ADDR(11);
            break;
    }
    
    // Copy the map data
    if (mapPtr) {
        for (int y = 0; y < height && y < 32; y++) {
            for (int x = 0; x < width && x < 32; x++) {
                mapPtr[y * 32 + x] = map[y * width + x];
            }
        }
    }
}

// Load palette for backgrounds
void loadBackgroundPalette(const unsigned short* palette, int count) {
    volatile unsigned short* paletteMemory = (volatile unsigned short*)PALETTE_BASE;
    
    // Load palette data into the background palette area
    for (int i = 0; i < count && i < 256; i++) {
        paletteMemory[i] = palette[i];
    }
}
