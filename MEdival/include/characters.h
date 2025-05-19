#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "gba.h"
#include "items.h"

// Character classes
typedef enum {
    CLASS_BERSERKER,
    CLASS_PALADIN,
    CLASS_ARCHER
} CharacterClass;

// Weapon types
typedef enum {
    WEAPON_BATTLE_AXE,
    WEAPON_SWORD,
    WEAPON_BOW
} WeaponType;

// Weapon evolution levels
typedef enum {
    EVOLUTION_BASIC,
    EVOLUTION_IMPROVED,
    EVOLUTION_MASTER
} WeaponEvolution;

// Weapon structure
typedef struct {
    WeaponType type;
    WeaponEvolution evolution;
    char name[20];
    int baseDamage;
    int durability;
    int maxDurability;
    int trainingPoints;
    int requiredTrainingForNextLevel;
} Weapon;

// Armor structure
typedef struct {
    char name[20];
    int defense;
    int durability;
    int maxDurability;
} Armor;

// Player stats
typedef struct {
    int hp;
    int maxHp;
    int attack;
    int defense;
    int speed;
} Stats;

// Player structure
typedef struct {
    CharacterClass class;
    char name[20];
    int x;
    int y;
    int direction;
    Stats stats;
    Weapon weapon;
    Armor armor;
    Item inventory[20];
    int inventoryCount;
    int coins;
    int defeatedEnemies;
} Player;

// Enemy structure
typedef struct {
    char name[20];
    int hp;
    int maxHp;
    int attack;
    int defense;
    int speed;
    int rewardCoins;
    Item droppedItem;
    int dropRate; // Percentage chance to drop the item (0-100)
} Enemy;

// Initialize a new player with the given class
void initPlayer(Player* player, CharacterClass class);

// Initialize weapons for each class
void initWeapon(Weapon* weapon, WeaponType type);

// Level up weapon evolution if training requirements are met
int evolveWeapon(Weapon* weapon);

// Check if weapon can evolve to next level
int canWeaponEvolve(Weapon* weapon);

// Update player stats based on equipment and class

// Update player stats based on equipment and class
void updatePlayerStats(Player* player);

// Enemy functions
void initEnemy(Enemy* enemy, int level);
void generateRandomEnemy(Enemy* enemy, int level);

// Player utility functions
void addItemToInventory(Player* player, Item item);
int removeItemFromInventory(Player* player, int index);
void useItem(Player* player, int itemIndex);
void repairArmor(Player* player, int amount);
void healPlayer(Player* player, int amount);

// Character movement
void movePlayer(Player* player, int dx, int dy);

#endif // CHARACTERS_H
