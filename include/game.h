#ifndef GAME_H
#define GAME_H

#include "gba.h"
#include "characters.h"
#include "maps.h"

// Game states
typedef enum {
    STATE_TITLE,
    STATE_CLASS_SELECT,
    STATE_WORLD_MAP,
    STATE_BATTLE,
    STATE_SHOP,
    STATE_INVENTORY,
    STATE_TRAINING,
    STATE_GAME_OVER,
    STATE_VICTORY
} GameState;

// Game context structure
typedef struct {
    GameState currentState;
    Player player;
    Map currentMap;
    int frame;
    int seed;
    int battleInProgress;
    int shopInProgress;
    int trainingInProgress;
    Enemy currentEnemy;
    int npcInteractionActive;
    int forcedEncounterActive;
    int menuCursor;
} GameContext;

// Initialize the game
void initGame();

// Update game logic based on current state
void updateGame();

// Render the game based on current state
void renderGame();

// Switch to a new game state
void setGameState(GameState newState);

// Get random number for game logic
int getRandomInt(int max);

// Main game loop handler
void gameLoop();

// Load game data (maps, enemies, items)
void loadGameData();

// Helper functions
void delay(int cycles);
void seedRandom(int seed);

extern GameContext gameContext;

#endif // GAME_H
