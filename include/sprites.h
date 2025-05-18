#ifndef SPRITES_H
#define SPRITES_H

#include "gba.h"

// Sprite directions
typedef enum {
    DIR_DOWN,
    DIR_UP,
    DIR_LEFT, 
    DIR_RIGHT
} Direction;

// Sprite animation frames
typedef struct {
    int frameCount;
    int* tileIndices;
    int animationSpeed;
    int currentFrame;
    int frameTimer;
} SpriteAnimation;

// Sprite structure
typedef struct {
    int x;
    int y;
    int width;
    int height;
    int oamIndex;
    Direction direction;
    SpriteAnimation animations[4]; // One per direction
} Sprite;

// Initialize sprite system
void initSpriteSystem();

// Create a new sprite
Sprite* createSprite(int x, int y, int width, int height);

// Update sprite animation
void updateSpriteAnimation(Sprite* sprite);

// Set sprite direction
void setSpriteDirection(Sprite* sprite, Direction dir);

// Move sprite
void moveSprite(Sprite* sprite, int dx, int dy);

// Set sprite position
void setSpritePosition(Sprite* sprite, int x, int y);

// Draw sprite
void drawSpriteFrame(Sprite* sprite);

// Hide sprite
void hideSprite(Sprite* sprite);

// Load sprite tiles
void loadSpriteTiles(const unsigned short* tiles, int tileCount);

// Load sprite palette
void loadSpritePalette(const unsigned short* palette, int paletteSize);

#endif // SPRITES_H
