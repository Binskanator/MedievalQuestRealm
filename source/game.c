#include "../include/game.h"
#include "../include/input.h"
#include "../include/graphics.h"
#include "../include/text.h"
#include "../include/sprites.h"
#include "../include/backgrounds.h"
#include "../include/combat.h"
#include "../include/shops.h"
#include "../include/training.h"
#include "../include/enemies.h"
#include "../include/maps.h"

// Game context instance (declared in game.h)
GameContext gameContext;

// Combat context for battles
CombatContext combatContext;

// Shop context for shop interactions
ShopContext shopContext;

// Training context for training interactions
TrainingContext trainingContext;

// Current shop
Shop currentShop;

// Initialize the game
void initGame() {
    // Start at title screen
    gameContext.currentState = STATE_TITLE;
    gameContext.frame = 0;
    gameContext.battleInProgress = 0;
    gameContext.shopInProgress = 0;
    gameContext.trainingInProgress = 0;
    gameContext.npcInteractionActive = 0;
    gameContext.forcedEncounterActive = 0;
    gameContext.menuCursor = 0;
    
    // Load title screen background
    loadBackground(BG_TITLE, 2);
    
    // Load game data (maps, enemies, items)
    loadGameData();
}

// Update game logic based on current state
void updateGame() {
    gameContext.frame++;
    
    switch (gameContext.currentState) {
        case STATE_TITLE:
            // Title screen logic
            if (keyPressed(&inputState, KEY_START)) {
                setGameState(STATE_CLASS_SELECT);
            }
            break;
            
        case STATE_CLASS_SELECT:
            // Class selection logic
            if (keyPressed(&inputState, KEY_UP)) {
                gameContext.menuCursor = (gameContext.menuCursor - 1 + 3) % 3;
            } else if (keyPressed(&inputState, KEY_DOWN)) {
                gameContext.menuCursor = (gameContext.menuCursor + 1) % 3;
            }
            
            if (keyPressed(&inputState, KEY_A)) {
                // Initialize player with selected class
                switch (gameContext.menuCursor) {
                    case 0:
                        initPlayer(&gameContext.player, CLASS_BERSERKER);
                        break;
                    case 1:
                        initPlayer(&gameContext.player, CLASS_PALADIN);
                        break;
                    case 2:
                        initPlayer(&gameContext.player, CLASS_ARCHER);
                        break;
                }
                
                // Load the starting map
                loadMap(&gameContext.currentMap, 0); // Map ID 0 = starting town
                
                // Start the game
                setGameState(STATE_WORLD_MAP);
            }
            break;
            
        case STATE_WORLD_MAP:
            // World map logic (player movement, etc.)
            int dx = 0, dy = 0;
            
            if (keyHeld(&inputState, KEY_LEFT)) dx = -1;
            else if (keyHeld(&inputState, KEY_RIGHT)) dx = 1;
            
            if (keyHeld(&inputState, KEY_UP)) dy = -1;
            else if (keyHeld(&inputState, KEY_DOWN)) dy = 1;
            
            if (dx != 0 || dy != 0) {
                // Move player if possible
                int newX = gameContext.player.x + dx;
                int newY = gameContext.player.y + dy;
                
                // Update player direction
                if (dx < 0) gameContext.player.direction = 2; // Left
                else if (dx > 0) gameContext.player.direction = 3; // Right
                else if (dy < 0) gameContext.player.direction = 1; // Up
                else if (dy > 0) gameContext.player.direction = 0; // Down
                
                // Check for map boundaries
                if (newX >= 0 && newX < gameContext.currentMap.width &&
                    newY >= 0 && newY < gameContext.currentMap.height) {
                    
                    // Check for walkable tiles
                    TileType tile = getTileAt(&gameContext.currentMap, newX, newY);
                    if (isTileWalkable(tile)) {
                        // Update player position
                        gameContext.player.x = newX;
                        gameContext.player.y = newY;
                        
                        // Check for map change
                        MapChange* mapChange = checkMapChange(&gameContext.currentMap, newX, newY);
                        if (mapChange) {
                            // Load new map and place player
                            loadMap(&gameContext.currentMap, mapChange->destMapId);
                            gameContext.player.x = mapChange->destX;
                            gameContext.player.y = mapChange->destY;
                        } else {
                            // Check for interactions based on tile type
                            switch (tile) {
                                case TILE_SHOP:
                                    if (keyPressed(&inputState, KEY_A)) {
                                        // Enter shop
                                        initShopWithType(&currentShop, SHOP_GENERAL);
                                        initShopContext(&shopContext, &currentShop, &gameContext.player);
                                        setGameState(STATE_SHOP);
                                    }
                                    break;
                                    
                                case TILE_TRAINING:
                                    if (keyPressed(&inputState, KEY_A)) {
                                        // Enter training
                                        initTraining(&trainingContext, &gameContext.player);
                                        setGameState(STATE_TRAINING);
                                    }
                                    break;
                                    
                                case TILE_HEAL:
                                    if (keyPressed(&inputState, KEY_A)) {
                                        // Heal player
                                        healPlayer(&gameContext.player, gameContext.player.stats.maxHp);
                                        repairArmor(&gameContext.player, gameContext.player.armor.maxDurability);
                                    }
                                    break;
                                    
                                default:
                                    // Check for NPCs at this position
                                    NPC* npc = findNpcAt(&gameContext.currentMap, newX, newY);
                                    if (npc && keyPressed(&inputState, KEY_A)) {
                                        gameContext.npcInteractionActive = 1;
                                        
                                        // If NPC has battle, start combat
                                        if (npc->hasBattle) {
                                            gameContext.forcedEncounterActive = 1;
                                            initEnemyByType(&gameContext.currentEnemy, 
                                                           getRandomEnemyType(npc->enemyLevel, npc->enemyLevel + 2),
                                                           npc->enemyLevel);
                                            initCombat(&combatContext, &gameContext.player, &gameContext.currentEnemy);
                                            setGameState(STATE_BATTLE);
                                        }
                                    }
                                    
                                    // Check for random enemy encounter in enemy spawn areas
                                    if (tile == TILE_ENEMY_SPAWN && checkEnemyEncounter(&gameContext.currentMap)) {
                                        // Generate random enemy
                                        int level = gameContext.currentMap.enemyMinLevel + 
                                                  getRandomInt(gameContext.currentMap.enemyMaxLevel - 
                                                             gameContext.currentMap.enemyMinLevel + 1);
                                        
                                        generateRandomEnemy(&gameContext.currentEnemy, level);
                                        initCombat(&combatContext, &gameContext.player, &gameContext.currentEnemy);
                                        setGameState(STATE_BATTLE);
                                    }
                                    break;
                            }
                        }
                    }
                }
            }
            
            // Inventory access
            if (keyPressed(&inputState, KEY_SELECT)) {
                setGameState(STATE_INVENTORY);
            }
            
            // Update map state (NPCs, etc.)
            updateMap(&gameContext.currentMap);
            break;
            
        case STATE_BATTLE:
            // Battle logic
            updateCombat(&combatContext);
            
            // Check if combat is over
            if (isCombatOver(&combatContext)) {
                if (combatContext.state == COMBAT_VICTORY) {
                    // Apply rewards to player
                    applyVictoryRewards(&combatContext);
                    
                    // Apply weapon training
                    applyWeaponTraining(&gameContext.player, 1 + getRandomInt(3));
                    
                    // Increment defeated enemies counter
                    gameContext.player.defeatedEnemies++;
                    
                    // Check if weapon can evolve
                    if (canWeaponEvolve(&gameContext.player.weapon)) {
                        evolveWeapon(&gameContext.player.weapon);
                    }
                }
                
                // Return to world map
                setGameState(STATE_WORLD_MAP);
            }
            break;
            
        case STATE_SHOP:
            // Shop logic
            updateShop(&shopContext);
            
            // Exit shop
            if (keyPressed(&inputState, KEY_B)) {
                setGameState(STATE_WORLD_MAP);
            }
            break;
            
        case STATE_INVENTORY:
            // Inventory navigation logic
            if (keyPressed(&inputState, KEY_UP)) {
                if (gameContext.menuCursor > 0) {
                    gameContext.menuCursor--;
                }
            } else if (keyPressed(&inputState, KEY_DOWN)) {
                if (gameContext.menuCursor < gameContext.player.inventoryCount - 1) {
                    gameContext.menuCursor++;
                }
            }
            
            // Use selected item
            if (keyPressed(&inputState, KEY_A) && gameContext.player.inventoryCount > 0) {
                useItem(&gameContext.player, gameContext.menuCursor);
            }
            
            // Exit inventory
            if (keyPressed(&inputState, KEY_B) || keyPressed(&inputState, KEY_SELECT)) {
                setGameState(STATE_WORLD_MAP);
            }
            break;
            
        case STATE_TRAINING:
            // Training logic
            updateTraining(&trainingContext);
            
            // Exit training
            if (!trainingContext.trainingInProgress && keyPressed(&inputState, KEY_B)) {
                setGameState(STATE_WORLD_MAP);
            }
            break;
            
        case STATE_GAME_OVER:
            // Game over logic
            if (keyPressed(&inputState, KEY_START)) {
                // Restart the game
                initGame();
            }
            break;
            
        case STATE_VICTORY:
            // Game victory logic
            if (keyPressed(&inputState, KEY_START)) {
                // Restart the game
                initGame();
            }
            break;
    }
}

// Render the game based on current state
void renderGame() {
    switch (gameContext.currentState) {
        case STATE_TITLE:
            // Draw title screen
            fillScreen(BLACK);
            printTextAligned(SCREEN_WIDTH / 2, 50, "MEDIEVAL RPG", WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            printTextAligned(SCREEN_WIDTH / 2, 80, "Press START", WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            break;
            
        case STATE_CLASS_SELECT:
            // Draw class selection screen
            fillScreen(BLACK);
            printTextAligned(SCREEN_WIDTH / 2, 20, "SELECT YOUR CLASS", WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            
            // Draw class options with cursor
            Color berserkerColor = (gameContext.menuCursor == 0) ? YELLOW : WHITE;
            Color paladinColor = (gameContext.menuCursor == 1) ? YELLOW : WHITE;
            Color archerColor = (gameContext.menuCursor == 2) ? YELLOW : WHITE;
            
            printText(60, 50, "Berserker - Battle Axe", berserkerColor);
            printText(60, 70, "Paladin - Sword", paladinColor);
            printText(60, 90, "Archer - Bow", archerColor);
            
            // Draw cursor
            printText(45, 50 + gameContext.menuCursor * 20, ">", YELLOW);
            
            // Draw class description based on selection
            char* description;
            switch (gameContext.menuCursor) {
                case 0:
                    description = "High attack, low defense. Berserk rage!";
                    break;
                case 1:
                    description = "Balanced stats. Holy protection.";
                    break;
                case 2:
                    description = "High accuracy, lower HP. Critical shots.";
                    break;
            }
            
            printTextBox(40, 120, 160, 30, description, WHITE, BLACK, GRAY);
            break;
            
        case STATE_WORLD_MAP:
            // Draw the map
            renderMap(&gameContext.currentMap, gameContext.player.x, gameContext.player.y);
            
            // Draw player stats
            drawHollowRect(0, 0, 80, 30, WHITE);
            printText(5, 5, "HP:", WHITE);
            printNumber(30, 5, gameContext.player.stats.hp, WHITE);
            printText(45, 5, "/", WHITE);
            printNumber(55, 5, gameContext.player.stats.maxHp, WHITE);
            
            printText(5, 15, "Coins:", WHITE);
            printNumber(45, 15, gameContext.player.coins, YELLOW);
            break;
            
        case STATE_BATTLE:
            // Draw battle screen
            renderCombatUI(&combatContext);
            break;
            
        case STATE_SHOP:
            // Draw shop screen
            renderShopUI(&shopContext);
            break;
            
        case STATE_INVENTORY:
            // Draw inventory screen
            fillScreen(BLACK);
            printTextAligned(SCREEN_WIDTH / 2, 10, "INVENTORY", WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            
            // Draw items with cursor
            int startY = 30;
            int itemsPerPage = 8;
            int startIndex = 0;
            
            // Display inventory items
            for (int i = 0; i < gameContext.player.inventoryCount && i < itemsPerPage; i++) {
                int index = startIndex + i;
                if (index < gameContext.player.inventoryCount) {
                    Color itemColor = (gameContext.menuCursor == index) ? YELLOW : WHITE;
                    printText(60, startY + i * 15, gameContext.player.inventory[index].name, itemColor);
                    printText(160, startY + i * 15, "x", itemColor);
                    printNumber(170, startY + i * 15, gameContext.player.inventory[index].count, itemColor);
                    
                    // Draw cursor
                    if (gameContext.menuCursor == index) {
                        printText(45, startY + i * 15, ">", YELLOW);
                    }
                }
            }
            
            // Draw selected item description
            if (gameContext.player.inventoryCount > 0) {
                drawHollowRect(20, 140, 200, 30, WHITE);
                printText(30, 150, gameContext.player.inventory[gameContext.menuCursor].description, WHITE);
            } else {
                drawHollowRect(20, 140, 200, 30, WHITE);
                printText(30, 150, "Inventory is empty", GRAY);
            }
            break;
            
        case STATE_TRAINING:
            // Draw training screen
            renderTrainingUI(&trainingContext);
            break;
            
        case STATE_GAME_OVER:
            // Draw game over screen
            fillScreen(BLACK);
            printTextAligned(SCREEN_WIDTH / 2, 60, "GAME OVER", RED, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            printTextAligned(SCREEN_WIDTH / 2, 90, "Press START to try again", WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            break;
            
        case STATE_VICTORY:
            // Draw victory screen
            fillScreen(BLACK);
            printTextAligned(SCREEN_WIDTH / 2, 60, "VICTORY!", GREEN, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            printTextAligned(SCREEN_WIDTH / 2, 90, "You have completed the game!", WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            printTextAligned(SCREEN_WIDTH / 2, 110, "Press START to play again", WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            break;
    }
}

// Switch to a new game state
void setGameState(GameState newState) {
    // Process any cleanup from the current state
    switch (gameContext.currentState) {
        case STATE_BATTLE:
            gameContext.battleInProgress = 0;
            break;
        case STATE_SHOP:
            gameContext.shopInProgress = 0;
            break;
        case STATE_TRAINING:
            gameContext.trainingInProgress = 0;
            break;
        default:
            break;
    }
    
    // Process any setup for the new state
    switch (newState) {
        case STATE_TITLE:
            loadBackground(BG_TITLE, 2);
            break;
        case STATE_WORLD_MAP:
            loadBackground(BG_FIELD, 2);
            break;
        case STATE_BATTLE:
            loadBackground(BG_BATTLE, 2);
            gameContext.battleInProgress = 1;
            break;
        case STATE_SHOP:
            loadBackground(BG_SHOP, 2);
            gameContext.shopInProgress = 1;
            break;
        case STATE_TRAINING:
            loadBackground(BG_TRAINING, 2);
            gameContext.trainingInProgress = 1;
            break;
        default:
            break;
    }
    
    // Set the new state
    gameContext.currentState = newState;
    
    // Reset menu cursor when changing states
    gameContext.menuCursor = 0;
}

// Get random number for game logic
int getRandomInt(int max) {
    // Simple LCG random number generator
    gameContext.seed = (gameContext.seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return (gameContext.seed % max);
}

// Load game data (maps, enemies, items)
void loadGameData() {
    // This would normally load from ROM data
    // For now we'll initialize with default values
}

// Helper functions
void delay(int cycles) {
    volatile int i;
    for (i = 0; i < cycles; i++);
}

void seedRandom(int seed) {
    gameContext.seed = seed;
}
