#ifndef MAPS_H
#define MAPS_H

#include "gba.h"

// Map tile types
typedef enum {
    TILE_EMPTY,
    TILE_WALL,
    TILE_GRASS,
    TILE_WATER,
    TILE_MOUNTAIN,
    TILE_FOREST,
    TILE_ROAD,
    TILE_SAND,
    TILE_SHOP,
    TILE_TRAINING,
    TILE_HEAL,
    TILE_TOWN,
    TILE_CASTLE,
    TILE_DUNGEON,
    TILE_ENEMY_SPAWN
} TileType;

// Map location types
typedef enum {
    LOCATION_FIELD,
    LOCATION_TOWN,
    LOCATION_CASTLE,
    LOCATION_DUNGEON,
    LOCATION_FOREST,
    LOCATION_MOUNTAINS
} LocationType;

// NPC structure
typedef struct {
    int x;
    int y;
    int direction;
    char name[20];
    char dialog[100];
    int isEnemy;
    int enemyLevel;
    int hasBattle;
    int hasTreasure;
    int treasureItemType;
} NPC;

// Map structure
typedef struct {
    int width;
    int height;
    char name[20];
    LocationType locationType;
    unsigned char* data;
    int enemyEncounterRate; // 0-100% chance per step in enemy areas
    int enemyMinLevel;
    int enemyMaxLevel;
    int shopPresent;
    int trainingPresent;
    int healingPresent;
    NPC npcs[10];
    int npcCount;
    int xOffset;
    int yOffset;
} Map;

// Map change structure
typedef struct {
    int sourceX;
    int sourceY;
    int destMapId;
    int destX;
    int destY;
} MapChange;

// Initialize a map
void initMap(Map* map, LocationType type);

// Generate a procedural map
void generateMap(Map* map, LocationType type, int width, int height);

// Load a predefined map
void loadMap(Map* map, int mapId);

// Check if a tile is walkable
int isTileWalkable(TileType tile);

// Get tile at map location
TileType getTileAt(Map* map, int x, int y);

// Set tile at map location
void setTileAt(Map* map, int x, int y, TileType tile);

// Check for map change
MapChange* checkMapChange(Map* map, int x, int y);

// Find NPC at position
NPC* findNpcAt(Map* map, int x, int y);

// Check for random enemy encounter
int checkEnemyEncounter(Map* map);

// Render map on screen
void renderMap(Map* map, int playerX, int playerY);

// Update map state (e.g., NPC movements)
void updateMap(Map* map);

// Map database
extern Map MAP_DATABASE[];
extern const int MAP_DATABASE_SIZE;

#endif // MAPS_H
