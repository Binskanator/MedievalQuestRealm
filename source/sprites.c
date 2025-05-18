#include "../include/sprites.h"
#include <stdlib.h>

// Sprite system state
#define MAX_SPRITES 16
Sprite sprites[MAX_SPRITES];
int spriteCount = 0;

// Animation data for the different classes and directions
// Each class has 4 directions, each with multiple frames
int berserkerTiles[4][4] = {
    {0, 1, 2, 1},  // Down
    {4, 5, 6, 5},  // Up
    {8, 9, 10, 9}, // Left
    {12, 13, 14, 13} // Right
};

int paladinTiles[4][4] = {
    {16, 17, 18, 17},  // Down
    {20, 21, 22, 21},  // Up
    {24, 25, 26, 25},  // Left
    {28, 29, 30, 29}   // Right
};

int archerTiles[4][4] = {
    {32, 33, 34, 33},  // Down
    {36, 37, 38, 37},  // Up
    {40, 41, 42, 41},  // Left
    {44, 45, 46, 45}   // Right
};

int enemyTiles[10][4] = {
    {48, 49, 50, 49},  // Goblin
    {52, 53, 54, 53},  // Orc
    {56, 57, 58, 57},  // Skeleton
    {60, 61, 62, 61},  // Zombie
    {64, 65, 66, 65},  // Bandit
    {68, 69, 70, 69},  // Wolf
    {72, 73, 74, 73},  // Troll
    {76, 77, 78, 77},  // Ghost
    {80, 81, 82, 81},  // Dragon
    {84, 85, 86, 85}   // Dark Knight
};

int npcTiles[4][4] = {
    {88, 89, 90, 89},  // Down
    {92, 93, 94, 93},  // Up
    {96, 97, 98, 97},  // Left
    {100, 101, 102, 101}  // Right
};

// Initialize sprite system
void initSpriteSystem() {
    spriteCount = 0;
    
    // Hide all hardware sprites initially
    for (int i = 0; i < 128; i++) {
        OAM[i].attr0 = ATTR0_HIDE;
    }
}

// Create a new sprite
Sprite* createSprite(int x, int y, int width, int height) {
    if (spriteCount >= MAX_SPRITES) {
        return NULL;
    }
    
    Sprite* sprite = &sprites[spriteCount];
    sprite->x = x;
    sprite->y = y;
    sprite->width = width;
    sprite->height = height;
    sprite->oamIndex = spriteCount;
    sprite->direction = DIR_DOWN;
    
    // Initialize animations for each direction
    for (int dir = 0; dir < 4; dir++) {
        sprite->animations[dir].frameCount = 4;
        sprite->animations[dir].currentFrame = 0;
        sprite->animations[dir].frameTimer = 0;
        sprite->animations[dir].animationSpeed = 10;
        
        // Allocate memory for the frame indices
        sprite->animations[dir].tileIndices = (int*)malloc(4 * sizeof(int));
        
        // Default tile indices
        for (int i = 0; i < 4; i++) {
            sprite->animations[dir].tileIndices[i] = dir * 4 + i;
        }
    }
    
    spriteCount++;
    return sprite;
}

// Update sprite animation
void updateSpriteAnimation(Sprite* sprite) {
    SpriteAnimation* currentAnim = &sprite->animations[sprite->direction];
    
    // Increment frame timer
    currentAnim->frameTimer++;
    
    // Change frame if it's time
    if (currentAnim->frameTimer >= currentAnim->animationSpeed) {
        currentAnim->frameTimer = 0;
        currentAnim->currentFrame = (currentAnim->currentFrame + 1) % currentAnim->frameCount;
    }
}

// Set sprite direction
void setSpriteDirection(Sprite* sprite, Direction dir) {
    if (dir >= 0 && dir < 4) {
        sprite->direction = dir;
    }
}

// Move sprite
void moveSprite(Sprite* sprite, int dx, int dy) {
    sprite->x += dx;
    sprite->y += dy;
    
    // Update direction based on movement
    if (dx < 0) {
        setSpriteDirection(sprite, DIR_LEFT);
    } else if (dx > 0) {
        setSpriteDirection(sprite, DIR_RIGHT);
    } else if (dy < 0) {
        setSpriteDirection(sprite, DIR_UP);
    } else if (dy > 0) {
        setSpriteDirection(sprite, DIR_DOWN);
    }
}

// Set sprite position
void setSpritePosition(Sprite* sprite, int x, int y) {
    sprite->x = x;
    sprite->y = y;
}

// Draw sprite
void drawSpriteFrame(Sprite* sprite) {
    // Get current animation and frame
    SpriteAnimation* currentAnim = &sprite->animations[sprite->direction];
    int currentTileIndex = currentAnim->tileIndices[currentAnim->currentFrame];
    
    // Update OAM entry
    OBJ_ATTR* oam = &OAM[sprite->oamIndex];
    
    // Set attributes
    oam->attr0 = (sprite->y & 0xFF) | ATTR0_SQUARE;
    oam->attr1 = (sprite->x & 0x1FF) | ATTR1_SIZE_16;
    oam->attr2 = currentTileIndex | ATTR2_PALROW(0);
}

// Hide sprite
void hideSprite(Sprite* sprite) {
    OAM[sprite->oamIndex].attr0 = ATTR0_HIDE;
}

// Load sprite tiles
void loadSpriteTiles(const unsigned short* tiles, int tileCount) {
    volatile unsigned short* tileMemory = (volatile unsigned short*)TILE_BASE_ADDR(4);
    
    // Copy tile data to VRAM
    for (int i = 0; i < tileCount * 16; i++) {
        tileMemory[i] = tiles[i];
    }
}

// Load sprite palette
void loadSpritePalette(const unsigned short* palette, int paletteSize) {
    volatile unsigned short* paletteMemory = (volatile unsigned short*)(PALETTE_BASE + 0x200);
    
    // Copy palette data to sprite palette area
    for (int i = 0; i < paletteSize; i++) {
        paletteMemory[i] = palette[i];
    }
}

// Create a player sprite based on character class
Sprite* createPlayerSprite(CharacterClass class) {
    Sprite* sprite = createSprite(120, 80, 16, 16);
    
    if (sprite) {
        // Set animation frames based on character class
        for (int dir = 0; dir < 4; dir++) {
            for (int i = 0; i < 4; i++) {
                switch (class) {
                    case CLASS_BERSERKER:
                        sprite->animations[dir].tileIndices[i] = berserkerTiles[dir][i];
                        break;
                    case CLASS_PALADIN:
                        sprite->animations[dir].tileIndices[i] = paladinTiles[dir][i];
                        break;
                    case CLASS_ARCHER:
                        sprite->animations[dir].tileIndices[i] = archerTiles[dir][i];
                        break;
                }
            }
        }
    }
    
    return sprite;
}

// Create an enemy sprite based on enemy type
Sprite* createEnemySprite(EnemyType type) {
    Sprite* sprite = createSprite(120, 40, 16, 16);
    
    if (sprite) {
        // Set animation frames based on enemy type
        for (int dir = 0; dir < 4; dir++) {
            for (int i = 0; i < 4; i++) {
                sprite->animations[dir].tileIndices[i] = enemyTiles[type][i];
            }
        }
    }
    
    return sprite;
}

// Create an NPC sprite
Sprite* createNPCSprite(int x, int y) {
    Sprite* sprite = createSprite(x, y, 16, 16);
    
    if (sprite) {
        // Set animation frames for NPC
        for (int dir = 0; dir < 4; dir++) {
            for (int i = 0; i < 4; i++) {
                sprite->animations[dir].tileIndices[i] = npcTiles[dir][i];
            }
        }
    }
    
    return sprite;
}

// Update all sprites
void updateSprites() {
    for (int i = 0; i < spriteCount; i++) {
        updateSpriteAnimation(&sprites[i]);
        drawSpriteFrame(&sprites[i]);
    }
}
