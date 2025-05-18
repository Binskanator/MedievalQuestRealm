#include "../include/characters.h"
#include <string.h>

// Initialize a new player with the given class
void initPlayer(Player* player, CharacterClass class) {
    player->class = class;
    player->x = 5;  // Starting position
    player->y = 5;
    player->direction = 0;  // Facing down
    player->coins = 100;    // Starting money
    player->defeatedEnemies = 0;
    player->inventoryCount = 0;
    
    // Initialize inventory with a basic healing potion
    Item startingPotion;
    initItem(&startingPotion, ITEM_POTION);
    startingPotion.count = 3;
    addItemToInventory(player, startingPotion);
    
    // Set class-specific stats and weapon
    switch (class) {
        case CLASS_BERSERKER:
            strcpy(player->name, "Berserker");
            player->stats.maxHp = 120;
            player->stats.hp = 120;
            player->stats.attack = 20;
            player->stats.defense = 8;
            player->stats.speed = 7;
            
            // Initialize battle axe
            initWeapon(&player->weapon, WEAPON_BATTLE_AXE);
            break;
            
        case CLASS_PALADIN:
            strcpy(player->name, "Paladin");
            player->stats.maxHp = 100;
            player->stats.hp = 100;
            player->stats.attack = 15;
            player->stats.defense = 15;
            player->stats.speed = 8;
            
            // Initialize sword
            initWeapon(&player->weapon, WEAPON_SWORD);
            break;
            
        case CLASS_ARCHER:
            strcpy(player->name, "Archer");
            player->stats.maxHp = 85;
            player->stats.hp = 85;
            player->stats.attack = 18;
            player->stats.defense = 10;
            player->stats.speed = 15;
            
            // Initialize bow
            initWeapon(&player->weapon, WEAPON_BOW);
            break;
    }
    
    // Initialize basic armor
    strcpy(player->armor.name, "Leather Armor");
    player->armor.defense = 5;
    player->armor.durability = 50;
    player->armor.maxDurability = 50;
    
    // Update player stats based on equipment
    updatePlayerStats(player);
}

// Initialize weapons for each class
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

// Level up weapon evolution if training requirements are met
int evolveWeapon(Weapon* weapon) {
    // Check if weapon can be evolved further
    if (weapon->evolution == EVOLUTION_MASTER) {
        return 0; // Already at maximum evolution
    }
    
    // Check if enough training points accumulated
    if (weapon->trainingPoints < weapon->requiredTrainingForNextLevel) {
        return 0; // Not enough training points
    }
    
    // Reset training points
    weapon->trainingPoints -= weapon->requiredTrainingForNextLevel;
    
    // Evolve the weapon
    switch (weapon->type) {
        case WEAPON_BATTLE_AXE:
            if (weapon->evolution == EVOLUTION_BASIC) {
                // Evolve to improved
                weapon->evolution = EVOLUTION_IMPROVED;
                strcpy(weapon->name, "Steel Battle Axe");
                weapon->baseDamage = 25;
                weapon->maxDurability = 60;
                weapon->durability = weapon->maxDurability;
                weapon->requiredTrainingForNextLevel = 25;
                return 1;
            } else if (weapon->evolution == EVOLUTION_IMPROVED) {
                // Evolve to master
                weapon->evolution = EVOLUTION_MASTER;
                strcpy(weapon->name, "Berserker's Executioner");
                weapon->baseDamage = 40;
                weapon->maxDurability = 80;
                weapon->durability = weapon->maxDurability;
                return 1;
            }
            break;
            
        case WEAPON_SWORD:
            if (weapon->evolution == EVOLUTION_BASIC) {
                // Evolve to improved
                weapon->evolution = EVOLUTION_IMPROVED;
                strcpy(weapon->name, "Knight's Longsword");
                weapon->baseDamage = 20;
                weapon->maxDurability = 70;
                weapon->durability = weapon->maxDurability;
                weapon->requiredTrainingForNextLevel = 25;
                return 1;
            } else if (weapon->evolution == EVOLUTION_IMPROVED) {
                // Evolve to master
                weapon->evolution = EVOLUTION_MASTER;
                strcpy(weapon->name, "Holy Avenger");
                weapon->baseDamage = 35;
                weapon->maxDurability = 90;
                weapon->durability = weapon->maxDurability;
                return 1;
            }
            break;
            
        case WEAPON_BOW:
            if (weapon->evolution == EVOLUTION_BASIC) {
                // Evolve to improved
                weapon->evolution = EVOLUTION_IMPROVED;
                strcpy(weapon->name, "Composite Bow");
                weapon->baseDamage = 18;
                weapon->maxDurability = 50;
                weapon->durability = weapon->maxDurability;
                weapon->requiredTrainingForNextLevel = 25;
                return 1;
            } else if (weapon->evolution == EVOLUTION_IMPROVED) {
                // Evolve to master
                weapon->evolution = EVOLUTION_MASTER;
                strcpy(weapon->name, "Elven Longbow");
                weapon->baseDamage = 30;
                weapon->maxDurability = 70;
                weapon->durability = weapon->maxDurability;
                return 1;
            }
            break;
    }
    
    return 0;
}

// Update player stats based on equipment and class
void updatePlayerStats(Player* player) {
    // Start with base stats for class
    Stats baseStats;
    
    switch (player->class) {
        case CLASS_BERSERKER:
            baseStats.maxHp = 120;
            baseStats.attack = 20;
            baseStats.defense = 8;
            baseStats.speed = 7;
            break;
            
        case CLASS_PALADIN:
            baseStats.maxHp = 100;
            baseStats.attack = 15;
            baseStats.defense = 15;
            baseStats.speed = 8;
            break;
            
        case CLASS_ARCHER:
            baseStats.maxHp = 85;
            baseStats.attack = 18;
            baseStats.defense = 10;
            baseStats.speed = 15;
            break;
    }
    
    // Apply equipment modifiers
    player->stats.maxHp = baseStats.maxHp;
    player->stats.attack = baseStats.attack + player->weapon.baseDamage;
    player->stats.defense = baseStats.defense + player->armor.defense;
    player->stats.speed = baseStats.speed;
    
    // Apply weapon evolution bonuses
    if (player->weapon.evolution == EVOLUTION_IMPROVED) {
        player->stats.attack += 5;
    } else if (player->weapon.evolution == EVOLUTION_MASTER) {
        player->stats.attack += 10;
        player->stats.speed += 2;
    }
    
    // Ensure HP doesn't exceed max
    if (player->stats.hp > player->stats.maxHp) {
        player->stats.hp = player->stats.maxHp;
    }
}

// Enemy functions
void initEnemy(Enemy* enemy, int level) {
    // Default enemy stats
    strcpy(enemy->name, "Goblin");
    enemy->maxHp = 20 + level * 5;
    enemy->hp = enemy->maxHp;
    enemy->attack = 5 + level * 2;
    enemy->defense = 3 + level;
    enemy->speed = 5 + level / 2;
    enemy->rewardCoins = 10 + level * 5;
    
    // Default dropped item (none)
    enemy->droppedItem.type = ITEM_NONE;
    enemy->dropRate = 0;
}

void generateRandomEnemy(Enemy* enemy, int level) {
    // Generate a random enemy based on level
    EnemyType type = getRandomEnemyType(level, level);
    initEnemyByType(enemy, type, level);
}

// Player utility functions
void addItemToInventory(Player* player, Item item) {
    // Check if item already exists in inventory
    for (int i = 0; i < player->inventoryCount; i++) {
        if (player->inventory[i].type == item.type) {
            // Add to existing stack
            player->inventory[i].count += item.count;
            return;
        }
    }
    
    // Add as new item if inventory has space
    if (player->inventoryCount < 20) {
        player->inventory[player->inventoryCount] = item;
        player->inventoryCount++;
    }
}

int removeItemFromInventory(Player* player, int index) {
    // Check if index is valid
    if (index < 0 || index >= player->inventoryCount) {
        return 0;
    }
    
    // Reduce count or remove item entirely
    player->inventory[index].count--;
    
    if (player->inventory[index].count <= 0) {
        // Remove the item by shifting remaining items
        for (int i = index; i < player->inventoryCount - 1; i++) {
            player->inventory[i] = player->inventory[i + 1];
        }
        player->inventoryCount--;
    }
    
    return 1;
}

void useItem(Player* player, int itemIndex) {
    // Check if index is valid
    if (itemIndex < 0 || itemIndex >= player->inventoryCount) {
        return;
    }
    
    // Apply item effect
    applyItemEffect(&player->inventory[itemIndex], player);
    
    // Remove one of the item
    removeItemFromInventory(player, itemIndex);
}

void repairArmor(Player* player, int amount) {
    // Restore armor durability
    player->armor.durability += amount;
    
    // Cap at max durability
    if (player->armor.durability > player->armor.maxDurability) {
        player->armor.durability = player->armor.maxDurability;
    }
}

void healPlayer(Player* player, int amount) {
    // Restore player HP
    player->stats.hp += amount;
    
    // Cap at max HP
    if (player->stats.hp > player->stats.maxHp) {
        player->stats.hp = player->stats.maxHp;
    }
}

// Character movement
void movePlayer(Player* player, int dx, int dy) {
    player->x += dx;
    player->y += dy;
    
    // Update direction based on movement
    if (dx < 0) player->direction = 2; // Left
    else if (dx > 0) player->direction = 3; // Right
    else if (dy < 0) player->direction = 1; // Up
    else if (dy > 0) player->direction = 0; // Down
}
