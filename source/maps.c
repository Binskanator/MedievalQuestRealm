#include "../include/maps.h"
#include "../include/game.h"
#include <string.h>

// Map database (predefined maps)
Map MAP_DATABASE[10]; // Up to 10 maps
const int MAP_DATABASE_SIZE = 0;

// Map change database
MapChange MAP_CHANGES[20]; // Up to 20 map transitions
int MAP_CHANGE_COUNT = 0;

// Map data storage (for procedurally generated maps)
unsigned char mapData[3][64*64]; // Space for 3 maps of 64x64 tiles

// Initialize a map
void initMap(Map* map, LocationType type) {
    map->locationType = type;
    map->xOffset = 0;
    map->yOffset = 0;
    map->npcCount = 0;
    
    // Set defaults based on location type
    switch (type) {
        case LOCATION_FIELD:
            strcpy(map->name, "Grassy Plains");
            map->enemyEncounterRate = 20; // 20% chance per step in enemy areas
            map->enemyMinLevel = 1;
            map->enemyMaxLevel = 5;
            map->shopPresent = 0;
            map->trainingPresent = 0;
            map->healingPresent = 0;
            break;
            
        case LOCATION_TOWN:
            strcpy(map->name, "Village");
            map->enemyEncounterRate = 0; // No random encounters in town
            map->enemyMinLevel = 0;
            map->enemyMaxLevel = 0;
            map->shopPresent = 1;
            map->trainingPresent = 1;
            map->healingPresent = 1;
            break;
            
        case LOCATION_CASTLE:
            strcpy(map->name, "Castle");
            map->enemyEncounterRate = 10; // Occasional encounters
            map->enemyMinLevel = 5;
            map->enemyMaxLevel = 10;
            map->shopPresent = 1;
            map->trainingPresent = 1;
            map->healingPresent = 1;
            break;
            
        case LOCATION_DUNGEON:
            strcpy(map->name, "Dungeon");
            map->enemyEncounterRate = 30; // High encounter rate
            map->enemyMinLevel = 8;
            map->enemyMaxLevel = 15;
            map->shopPresent = 0;
            map->trainingPresent = 0;
            map->healingPresent = 0;
            break;
            
        case LOCATION_FOREST:
            strcpy(map->name, "Mystic Forest");
            map->enemyEncounterRate = 25;
            map->enemyMinLevel = 3;
            map->enemyMaxLevel = 8;
            map->shopPresent = 0;
            map->trainingPresent = 0;
            map->healingPresent = 0;
            break;
            
        case LOCATION_MOUNTAINS:
            strcpy(map->name, "Dragon Mountains");
            map->enemyEncounterRate = 20;
            map->enemyMinLevel = 10;
            map->enemyMaxLevel = 20;
            map->shopPresent = 0;
            map->trainingPresent = 0;
            map->healingPresent = 0;
            break;
    }
}

// Generate a procedural map
void generateMap(Map* map, LocationType type, int width, int height) {
    // Initialize map properties
    initMap(map, type);
    
    // Set dimensions
    map->width = width;
    map->height = height;
    
    // Allocate data for map (use one of the pre-allocated buffers)
    static int currentMapBuffer = 0;
    map->data = mapData[currentMapBuffer];
    currentMapBuffer = (currentMapBuffer + 1) % 3;
    
    // Fill map with appropriate tiles based on location type
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            TileType tile;
            
            // Border walls
            if (x == 0 || y == 0 || x == width-1 || y == height-1) {
                tile = TILE_WALL;
            } else {
                // Generate procedural terrain based on location type
                switch (type) {
                    case LOCATION_FIELD:
                        // Mostly grass with some water and roads
                        if (getRandomInt(100) < 5) {
                            tile = TILE_WATER;
                        } else if (getRandomInt(100) < 10) {
                            tile = TILE_ROAD;
                        } else {
                            tile = TILE_GRASS;
                        }
                        
                        // Add enemy spawn areas
                        if (getRandomInt(100) < 30) {
                            tile = TILE_ENEMY_SPAWN;
                        }
                        break;
                        
                    case LOCATION_TOWN:
                        // Roads, buildings, and services
                        if ((x % 8 < 2) || (y % 8 < 2)) {
                            tile = TILE_ROAD;
                        } else {
                            if (getRandomInt(100) < 20) {
                                tile = TILE_EMPTY; // Building interiors
                            } else {
                                tile = TILE_GRASS;
                            }
                        }
                        
                        // Add shop, training, and healing stations at specific spots
                        if (x == width/4 && y == height/4) {
                            tile = TILE_SHOP;
                        } else if (x == width/2 && y == height/2) {
                            tile = TILE_TRAINING;
                        } else if (x == 3*width/4 && y == 3*height/4) {
                            tile = TILE_HEAL;
                        }
                        break;
                        
                    case LOCATION_CASTLE:
                        // Structured castle layout
                        if ((x % 10 < 2) || (y % 10 < 2)) {
                            tile = TILE_WALL;
                        } else {
                            tile = TILE_EMPTY;
                        }
                        
                        // Add enemy encounters in some rooms
                        if ((x % 10 > 5) && (y % 10 > 5) && (getRandomInt(100) < 40)) {
                            tile = TILE_ENEMY_SPAWN;
                        }
                        
                        // Services at specific spots
                        if (x == width/2 && y == height/2) {
                            tile = TILE_SHOP;
                        } else if (x == width/3 && y == height/3) {
                            tile = TILE_TRAINING;
                        } else if (x == 2*width/3 && y == 2*height/3) {
                            tile = TILE_HEAL;
                        }
                        break;
                        
                    case LOCATION_DUNGEON:
                        // Maze-like dungeon
                        if ((x % 4 == 0) || (y % 4 == 0)) {
                            tile = TILE_WALL;
                        } else {
                            tile = TILE_EMPTY;
                            
                            // High concentration of enemy spawns
                            if (getRandomInt(100) < 60) {
                                tile = TILE_ENEMY_SPAWN;
                            }
                        }
                        
                        // Create some passages through walls
                        if (((x % 4 == 0) && (y % 4 == 2)) || 
                            ((y % 4 == 0) && (x % 4 == 2))) {
                            tile = TILE_EMPTY;
                        }
                        break;
                        
                    case LOCATION_FOREST:
                        // Dense forest with clearings
                        if (getRandomInt(100) < 60) {
                            tile = TILE_FOREST;
                        } else if (getRandomInt(100) < 10) {
                            tile = TILE_WATER; // Small ponds
                        } else {
                            tile = TILE_GRASS;
                        }
                        
                        // Add enemy spawns in certain areas
                        if (getRandomInt(100) < 40) {
                            tile = TILE_ENEMY_SPAWN;
                        }
                        break;
                        
                    case LOCATION_MOUNTAINS:
                        // Rocky terrain with paths
                        if (getRandomInt(100) < 50) {
                            tile = TILE_MOUNTAIN;
                        } else if (getRandomInt(100) < 20) {
                            tile = TILE_ROAD; // Mountain paths
                        } else {
                            tile = TILE_GRASS;
                        }
                        
                        // High-level enemy spawns
                        if (getRandomInt(100) < 45) {
                            tile = TILE_ENEMY_SPAWN;
                        }
                        
                        // Dragon's lair (boss area)
                        if (x > width*3/4 && y > height*3/4) {
                            if (getRandomInt(100) < 80) {
                                tile = TILE_DUNGEON;
                            }
                        }
                        break;
                }
            }
            
            // Set the tile
            map->data[y * width + x] = tile;
        }
    }
    
    // Add NPCs based on location type
    int npcCount = 0;
    switch (type) {
        case LOCATION_TOWN:
            npcCount = 5; // More NPCs in town
            break;
        case LOCATION_CASTLE:
            npcCount = 7; // Many NPCs in castle
            break;
        case LOCATION_FIELD:
        case LOCATION_FOREST:
            npcCount = 3; // Few NPCs in wilderness
            break;
        case LOCATION_DUNGEON:
        case LOCATION_MOUNTAINS:
            npcCount = 2; // Very few NPCs in dangerous areas
            break;
    }
    
    // Add NPCs to the map
    for (int i = 0; i < npcCount && map->npcCount < 10; i++) {
        // Find a valid position for the NPC
        int x, y;
        do {
            x = getRandomInt(width - 2) + 1;
            y = getRandomInt(height - 2) + 1;
            
            // Check if position is walkable and not on a special tile
            TileType tile = map->data[y * width + x];
        } while (!isTileWalkable(map->data[y * width + x]) || 
                 map->data[y * width + x] == TILE_SHOP ||
                 map->data[y * width + x] == TILE_TRAINING ||
                 map->data[y * width + x] == TILE_HEAL);
        
        // Create the NPC
        NPC npc;
        npc.x = x;
        npc.y = y;
        npc.direction = getRandomInt(4);
        
        // Set NPC properties based on location
        if (type == LOCATION_TOWN || type == LOCATION_CASTLE) {
            // Friendly NPCs in safe areas
            strcpy(npc.name, "Villager");
            strcpy(npc.dialog, "Welcome to our peaceful town!");
            npc.isEnemy = 0;
            npc.hasBattle = 0;
            
            // Some NPCs might give items
            if (getRandomInt(100) < 30) {
                npc.hasTreasure = 1;
                npc.treasureItemType = ITEM_POTION + getRandomInt(3);
                strcpy(npc.dialog, "Please take this. It might help you on your journey!");
            } else {
                npc.hasTreasure = 0;
            }
        } else {
            // Potential enemies in wilderness
            if (getRandomInt(100) < 50) {
                strcpy(npc.name, "Traveler");
                strcpy(npc.dialog, "Be careful of monsters in this area!");
                npc.isEnemy = 0;
                npc.hasBattle = 0;
                npc.hasTreasure = 0;
            } else {
                // Enemy NPC with forced battle
                strcpy(npc.name, "Bandit");
                strcpy(npc.dialog, "Prepare to fight!");
                npc.isEnemy = 1;
                npc.hasBattle = 1;
                npc.enemyLevel = map->enemyMinLevel + 
                               getRandomInt(map->enemyMaxLevel - map->enemyMinLevel + 1);
                npc.hasTreasure = 0;
            }
        }
        
        // Add NPC to map
        map->npcs[map->npcCount++] = npc;
    }
}

// Load a predefined map
void loadMap(Map* map, int mapId) {
    // For now, generate procedural maps
    // In a real game, this would load from predefined map data
    if (mapId >= 0 && mapId < MAP_DATABASE_SIZE) {
        // Copy from database
        *map = MAP_DATABASE[mapId];
    } else {
        // Generate appropriate map based on ID
        LocationType type;
        
        switch (mapId) {
            case 0: // Starting town
                type = LOCATION_TOWN;
                break;
            case 1: // First field area
                type = LOCATION_FIELD;
                break;
            case 2: // Forest
                type = LOCATION_FOREST;
                break;
            case 3: // Castle
                type = LOCATION_CASTLE;
                break;
            case 4: // Dungeon
                type = LOCATION_DUNGEON;
                break;
            case 5: // Mountains
                type = LOCATION_MOUNTAINS;
                break;
            default:
                type = LOCATION_FIELD;
                break;
        }
        
        // Generate a map of appropriate size
        generateMap(map, type, 64, 64);
        
        // If this is a new map, add it to the database
        if (MAP_DATABASE_SIZE < 10) {
            MAP_DATABASE[MAP_DATABASE_SIZE] = *map;
            MAP_DATABASE_SIZE++;
        }
    }
}

// Check if a tile is walkable
int isTileWalkable(TileType tile) {
    switch (tile) {
        case TILE_EMPTY:
        case TILE_GRASS:
        case TILE_ROAD:
        case TILE_SAND:
        case TILE_SHOP:
        case TILE_TRAINING:
        case TILE_HEAL:
        case TILE_TOWN:
        case TILE_ENEMY_SPAWN:
            return 1;
            
        case TILE_WALL:
        case TILE_WATER:
        case TILE_MOUNTAIN:
        case TILE_FOREST:
        case TILE_CASTLE:
        case TILE_DUNGEON:
            return 0;
            
        default:
            return 0;
    }
}

// Get tile at map location
TileType getTileAt(Map* map, int x, int y) {
    if (x >= 0 && x < map->width && y >= 0 && y < map->height) {
        return (TileType)map->data[y * map->width + x];
    }
    return TILE_WALL; // Default to wall for out of bounds
}

// Set tile at map location
void setTileAt(Map* map, int x, int y, TileType tile) {
    if (x >= 0 && x < map->width && y >= 0 && y < map->height) {
        map->data[y * map->width + x] = tile;
    }
}

// Check for map change
MapChange* checkMapChange(Map* map, int x, int y) {
    // Check if player is at a map transition point
    
    // Edge of map transitions
    if (x == 0 || y == 0 || x == map->width-1 || y == map->height-1) {
        // Generate transition to appropriate adjacent map
        static MapChange transition;
        
        // Determine destination map based on current map's type
        int destMapId;
        
        switch (map->locationType) {
            case LOCATION_TOWN:
                destMapId = 1; // Town -> Field
                break;
            case LOCATION_FIELD:
                if (getRandomInt(2) == 0) {
                    destMapId = 2; // Field -> Forest
                } else {
                    destMapId = 0; // Field -> Town
                }
                break;
            case LOCATION_FOREST:
                if (getRandomInt(2) == 0) {
                    destMapId = 3; // Forest -> Castle
                } else {
                    destMapId = 1; // Forest -> Field
                }
                break;
            case LOCATION_CASTLE:
                if (getRandomInt(2) == 0) {
                    destMapId = 4; // Castle -> Dungeon
                } else {
                    destMapId = 2; // Castle -> Forest
                }
                break;
            case LOCATION_DUNGEON:
                destMapId = 3; // Dungeon -> Castle
                break;
            case LOCATION_MOUNTAINS:
                destMapId = 1; // Mountains -> Field
                break;
            default:
                destMapId = 0;
                break;
        }
        
        // Set up transition details
        transition.sourceX = x;
        transition.sourceY = y;
        transition.destMapId = destMapId;
        
        // Place player on opposite side of new map
        if (x == 0) {
            transition.destX = MAP_DATABASE[destMapId].width - 2;
            transition.destY = y;
        } else if (x == map->width-1) {
            transition.destX = 1;
            transition.destY = y;
        } else if (y == 0) {
            transition.destX = x;
            transition.destY = MAP_DATABASE[destMapId].height - 2;
        } else {
            transition.destX = x;
            transition.destY = 1;
        }
        
        return &transition;
    }
    
    // Special map transition tiles (e.g., dungeon entrances)
    TileType tile = getTileAt(map, x, y);
    
    if (tile == TILE_DUNGEON) {
        static MapChange transition;
        
        transition.sourceX = x;
        transition.sourceY = y;
        transition.destMapId = 4; // Dungeon
        transition.destX = MAP_DATABASE[4].width / 2;
        transition.destY = MAP_DATABASE[4].height / 2;
        
        return &transition;
    }
    
    if (tile == TILE_CASTLE) {
        static MapChange transition;
        
        transition.sourceX = x;
        transition.sourceY = y;
        transition.destMapId = 3; // Castle
        transition.destX = MAP_DATABASE[3].width / 2;
        transition.destY = MAP_DATABASE[3].height / 2;
        
        return &transition;
    }
    
    if (tile == TILE_TOWN) {
        static MapChange transition;
        
        transition.sourceX = x;
        transition.sourceY = y;
        transition.destMapId = 0; // Town
        transition.destX = MAP_DATABASE[0].width / 2;
        transition.destY = MAP_DATABASE[0].height / 2;
        
        return &transition;
    }
    
    // Also check the predefined transitions
    for (int i = 0; i < MAP_CHANGE_COUNT; i++) {
        if (MAP_CHANGES[i].sourceX == x && MAP_CHANGES[i].sourceY == y) {
            return &MAP_CHANGES[i];
        }
    }
    
    return 0; // No transition
}

// Find NPC at position
NPC* findNpcAt(Map* map, int x, int y) {
    for (int i = 0; i < map->npcCount; i++) {
        if (map->npcs[i].x == x && map->npcs[i].y == y) {
            return &map->npcs[i];
        }
    }
    
    return 0; // No NPC found
}

// Check for random enemy encounter
int checkEnemyEncounter(Map* map) {
    // Roll for encounter based on the map's encounter rate
    return (getRandomInt(100) < map->enemyEncounterRate);
}

// Render map on screen
void renderMap(Map* map, int playerX, int playerY) {
    // Calculate viewport edges (center on player)
    int startX = playerX - 10;
    int startY = playerY - 7;
    int endX = startX + 20;
    int endY = startY + 14;
    
    // Clamp to map boundaries
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX > map->width) endX = map->width;
    if (endY > map->height) endY = map->height;
    
    // Clear screen
    fillScreen(BLACK);
    
    // Draw visible tiles
    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            // Calculate screen position
            int screenX = (x - startX) * 12;
            int screenY = (y - startY) * 12;
            
            // Get tile type
            TileType tile = getTileAt(map, x, y);
            
            // Draw tile
            Color tileColor;
            switch (tile) {
                case TILE_EMPTY:
                    tileColor = RGB5(8, 8, 8); // Dark gray
                    break;
                case TILE_WALL:
                    tileColor = RGB5(15, 15, 15); // Medium gray
                    break;
                case TILE_GRASS:
                    tileColor = RGB5(0, 20, 0); // Green
                    break;
                case TILE_WATER:
                    tileColor = RGB5(0, 0, 20); // Blue
                    break;
                case TILE_MOUNTAIN:
                    tileColor = RGB5(20, 10, 0); // Brown
                    break;
                case TILE_FOREST:
                    tileColor = RGB5(0, 10, 0); // Dark green
                    break;
                case TILE_ROAD:
                    tileColor = RGB5(20, 20, 10); // Tan
                    break;
                case TILE_SAND:
                    tileColor = RGB5(25, 25, 15); // Light tan
                    break;
                case TILE_SHOP:
                    tileColor = RGB5(20, 10, 20); // Purple
                    break;
                case TILE_TRAINING:
                    tileColor = RGB5(25, 0, 0); // Red
                    break;
                case TILE_HEAL:
                    tileColor = RGB5(0, 25, 0); // Bright green
                    break;
                case TILE_TOWN:
                    tileColor = RGB5(20, 20, 20); // Light gray
                    break;
                case TILE_CASTLE:
                    tileColor = RGB5(20, 20, 25); // Light blue-gray
                    break;
                case TILE_DUNGEON:
                    tileColor = RGB5(10, 0, 10); // Dark purple
                    break;
                case TILE_ENEMY_SPAWN:
                    tileColor = RGB5(0, 15, 0); // Medium green (like grass)
                    break;
                default:
                    tileColor = RGB5(0, 0, 0); // Black
                    break;
            }
            
            // Draw the tile
            drawRect(screenX, screenY, 11, 11, tileColor);
        }
    }
    
    // Draw NPCs
    for (int i = 0; i < map->npcCount; i++) {
        if (map->npcs[i].x >= startX && map->npcs[i].x < endX &&
            map->npcs[i].y >= startY && map->npcs[i].y < endY) {
            
            // Calculate screen position
            int screenX = (map->npcs[i].x - startX) * 12;
            int screenY = (map->npcs[i].y - startY) * 12;
            
            // Draw NPC (simple colored rectangle for now)
            if (map->npcs[i].isEnemy) {
                drawRect(screenX + 2, screenY + 2, 8, 8, RED);
            } else {
                drawRect(screenX + 2, screenY + 2, 8, 8, YELLOW);
            }
        }
    }
    
    // Draw player
    int playerScreenX = (playerX - startX) * 12;
    int playerScreenY = (playerY - startY) * 12;
    drawRect(playerScreenX + 2, playerScreenY + 2, 8, 8, BLUE);
    
    // Draw map name at top
    printTextAligned(SCREEN_WIDTH / 2, 155, map->name, WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
}

// Update map state (e.g., NPC movements)
void updateMap(Map* map) {
    // Move NPCs occasionally
    for (int i = 0; i < map->npcCount; i++) {
        // Only move NPCs every 60 frames (1 second) and with 30% chance
        if (gameContext.frame % 60 == 0 && getRandomInt(100) < 30) {
            // Pick a random direction
            int direction = getRandomInt(4);
            int dx = 0, dy = 0;
            
            switch (direction) {
                case 0: dy = -1; break; // Up
                case 1: dy = 1; break;  // Down
                case 2: dx = -1; break; // Left
                case 3: dx = 1; break;  // Right
            }
            
            // Update NPC direction
            map->npcs[i].direction = direction;
            
            // Check if new position is valid
            int newX = map->npcs[i].x + dx;
            int newY = map->npcs[i].y + dy;
            
            if (newX >= 0 && newX < map->width && newY >= 0 && newY < map->height) {
                TileType tile = getTileAt(map, newX, newY);
                
                if (isTileWalkable(tile)) {
                    // Check for collisions with other NPCs
                    int collision = 0;
                    for (int j = 0; j < map->npcCount; j++) {
                        if (i != j && map->npcs[j].x == newX && map->npcs[j].y == newY) {
                            collision = 1;
                            break;
                        }
                    }
                    
                    // Check for collision with player
                    if (newX == gameContext.player.x && newY == gameContext.player.y) {
                        collision = 1;
                    }
                    
                    // Move if no collision
                    if (!collision) {
                        map->npcs[i].x = newX;
                        map->npcs[i].y = newY;
                    }
                }
            }
        }
    }
}
