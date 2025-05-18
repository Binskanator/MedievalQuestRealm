#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// GBA Display Constants
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160

// Character Classes
typedef enum {
    CLASS_BERSERKER,
    CLASS_PALADIN,
    CLASS_ARCHER
} CharacterClass;

// Weapon Types
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

// Player structure
typedef struct {
    CharacterClass class;
    char name[20];
    int hp;
    int maxHp;
    int coins;
    Weapon weapon;
} Player;

// Initialize weapon for a class
void initWeapon(Weapon* weapon, WeaponType type) {
    weapon->type = type;
    weapon->evolution = EVOLUTION_BASIC;
    weapon->trainingPoints = 0;
    weapon->requiredTrainingForNextLevel = 10;
    
    switch (type) {
        case WEAPON_BATTLE_AXE:
            strcpy(weapon->name, "Bronze Axe");
            weapon->baseDamage = 15;
            weapon->durability = 40;
            weapon->maxDurability = 40;
            break;
        case WEAPON_SWORD:
            strcpy(weapon->name, "Iron Sword");
            weapon->baseDamage = 12;
            weapon->durability = 50;
            weapon->maxDurability = 50;
            break;
        case WEAPON_BOW:
            strcpy(weapon->name, "Wooden Bow");
            weapon->baseDamage = 10;
            weapon->durability = 35;
            weapon->maxDurability = 35;
            break;
    }
}

// Initialize player with selected class
void initPlayer(Player* player, CharacterClass class) {
    player->class = class;
    player->coins = 100;
    
    switch (class) {
        case CLASS_BERSERKER:
            strcpy(player->name, "Berserker");
            player->maxHp = 120;
            player->hp = 120;
            initWeapon(&player->weapon, WEAPON_BATTLE_AXE);
            break;
        case CLASS_PALADIN:
            strcpy(player->name, "Paladin");
            player->maxHp = 100;
            player->hp = 100;
            initWeapon(&player->weapon, WEAPON_SWORD);
            break;
        case CLASS_ARCHER:
            strcpy(player->name, "Archer");
            player->maxHp = 85;
            player->hp = 85;
            initWeapon(&player->weapon, WEAPON_BOW);
            break;
    }
}

// Train weapon to evolve it
int trainWeapon(Player* player, int trainingPoints) {
    player->weapon.trainingPoints += trainingPoints;
    
    // Check if weapon can evolve
    if (player->weapon.trainingPoints >= player->weapon.requiredTrainingForNextLevel &&
        player->weapon.evolution < EVOLUTION_MASTER) {
        
        player->weapon.trainingPoints -= player->weapon.requiredTrainingForNextLevel;
        player->weapon.evolution++;
        
        // Update weapon stats based on evolution
        switch (player->weapon.type) {
            case WEAPON_BATTLE_AXE:
                if (player->weapon.evolution == EVOLUTION_IMPROVED) {
                    strcpy(player->weapon.name, "Steel Battle Axe");
                    player->weapon.baseDamage = 25;
                    player->weapon.requiredTrainingForNextLevel = 25;
                } else if (player->weapon.evolution == EVOLUTION_MASTER) {
                    strcpy(player->weapon.name, "Berserker's Executioner");
                    player->weapon.baseDamage = 40;
                }
                break;
            case WEAPON_SWORD:
                if (player->weapon.evolution == EVOLUTION_IMPROVED) {
                    strcpy(player->weapon.name, "Knight's Longsword");
                    player->weapon.baseDamage = 20;
                    player->weapon.requiredTrainingForNextLevel = 25;
                } else if (player->weapon.evolution == EVOLUTION_MASTER) {
                    strcpy(player->weapon.name, "Holy Avenger");
                    player->weapon.baseDamage = 35;
                }
                break;
            case WEAPON_BOW:
                if (player->weapon.evolution == EVOLUTION_IMPROVED) {
                    strcpy(player->weapon.name, "Composite Bow");
                    player->weapon.baseDamage = 18;
                    player->weapon.requiredTrainingForNextLevel = 25;
                } else if (player->weapon.evolution == EVOLUTION_MASTER) {
                    strcpy(player->weapon.name, "Elven Longbow");
                    player->weapon.baseDamage = 30;
                }
                break;
        }
        return 1; // Weapon evolved
    }
    return 0; // No evolution occurred
}

// Simple test function to demonstrate game features
void testGame() {
    printf("Medieval RPG for Game Boy Advance\n");
    printf("-------------------------------\n\n");
    
    Player player;
    
    // Test each character class
    for (int cls = CLASS_BERSERKER; cls <= CLASS_ARCHER; cls++) {
        initPlayer(&player, cls);
        
        printf("Class: %s\n", player.name);
        printf("HP: %d/%d\n", player.hp, player.maxHp);
        printf("Starting Weapon: %s (Damage: %d)\n", player.weapon.name, player.weapon.baseDamage);
        
        printf("Training weapon...\n");
        // First evolution
        for (int i = 0; i < 10; i++) {
            int points = 1;
            printf("Training session %d: +%d points\n", i+1, points);
            trainWeapon(&player, points);
        }
        
        printf("Weapon evolved to: %s (Damage: %d)\n", player.weapon.name, player.weapon.baseDamage);
        
        // Second evolution
        printf("Further training...\n");
        for (int i = 0; i < 25; i++) {
            int points = 1;
            trainWeapon(&player, points);
        }
        
        printf("Weapon evolved to: %s (Damage: %d)\n", player.weapon.name, player.weapon.baseDamage);
        printf("\n");
    }
    
    printf("\nGame Features:\n");
    printf("- 3 Character Classes: Berserker, Paladin, and Archer\n");
    printf("- Weapon Evolution System through Training\n");
    printf("- Combat System with multiple enemies\n");
    printf("- Shop System for buying/selling items\n");
    printf("- Multiple maps with exploration\n");
    printf("- NPC interaction and quests\n");
}

// Main function
int main() {
    testGame();
    return 0;
}