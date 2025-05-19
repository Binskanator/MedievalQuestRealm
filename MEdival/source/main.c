/* 
 * Medieval RPG for the Game Boy Advance
 * A classic RPG with multiple character classes, weapon evolution,
 * training mechanics, and shop systems.
 */

#include <string.h>
#include "../include/gba.h"
#include "../include/game.h"
#include "../include/input.h"
#include "../include/graphics.h"
#include "../include/text.h"
#include "../include/sprites.h"
#include "../include/backgrounds.h"
#include "../include/globals.h"

int main() {
    // Setup video mode
    REG_DISPCNT = MODE_4 | BG2_ENABLE | OBJ_ENABLE | OBJ_MAP_1D;
    
    // Initialize systems
    initGraphics(GRAPHICS_MODE_TILE);
    initInput(&inputState);
    initTextSystem();
    initSpriteSystem();
    initBackgrounds();
    
    // Initialize game with a random seed
    // Use a simple value as initial seed (can be enhanced with timer)
    gameContext.seed = 1234;
    seedRandom(gameContext.seed);
    
    // Initialize the game itself
    initGame();
    
    // Main game loop
    while (1) {
        // Update input
        updateInput(&inputState);
        
        // Update game logic
        updateGame();
        
        // Wait for vertical blank before drawing
        waitForVBlank();
        
        // Render current game state
        renderGame();
        
        // Update sprite animations and positions
        updateSprites();
        
        // Update backgrounds if needed
        updateBackgrounds();
    }
    
    return 0;
}
