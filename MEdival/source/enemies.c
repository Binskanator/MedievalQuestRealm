#include "../include/enemies.h"
#include "../include/game.h"
#include <string.h>

// Enemy database
EnemyData ENEMY_DATABASE[] = {
    {ENEMY_GOBLIN, "Goblin", 25, 8, 3, 5, 10, ITEM_POTION, ITEM_ATTACK_BOOST, 20, ENEMY_NORMAL},
    {ENEMY_ORC, "Orc", 40, 12, 5, 3, 15, ITEM_POTION, ITEM_REPAIR_KIT, 15, ENEMY_NORMAL},
    {ENEMY_SKELETON, "Skeleton", 30, 10, 2, 8, 12, ITEM_POTION, ITEM_DEFENSE_BOOST, 10, ENEMY_NORMAL},
    {ENEMY_ZOMBIE, "Zombie", 45, 7, 7, 2, 20, ITEM_HI_POTION, ITEM_REPAIR_KIT, 25, ENEMY_NORMAL},
    {ENEMY_BANDIT, "Bandit", 35, 15, 4, 10, 30, ITEM_POTION, ITEM_TRAINING_MANUAL, 5, ENEMY_NORMAL},
    {ENEMY_WOLF, "Wolf", 20, 14, 2, 15, 10, ITEM_POTION, ITEM_ATTACK_BOOST, 15, ENEMY_NORMAL},
    {ENEMY_TROLL, "Troll", 60, 18, 10, 3, 40, ITEM_HI_POTION, ITEM_DEFENSE_BOOST, 20, ENEMY_NORMAL},
    {ENEMY_GHOST, "Ghost", 25, 12, 6, 12, 25, ITEM_HI_POTION, ITEM_REPAIR_KIT, 30, ENEMY_NORMAL},
    {ENEMY_DRAGON, "Dragon", 100, 25, 15, 5, 100, ITEM_SUPER_POTION, ITEM_TRAINING_MANUAL, 50, ENEMY_BOSS},
    {ENEMY_DARK_KNIGHT, "Dark Knight", 80, 20, 12, 8, 75, ITEM_SUPER_POTION, ITEM_TRAINING_MANUAL, 40, ENEMY_BOSS}
};

const int ENEMY_DATABASE_SIZE = sizeof(ENEMY_DATABASE) / sizeof(EnemyData);

// Get enemy data by type
EnemyData* getEnemyData(EnemyType type) {
    for (int i = 0; i < ENEMY_DATABASE_SIZE; i++) {
        if (ENEMY_DATABASE[i].type == type) {
            return &ENEMY_DATABASE[i];
        }
    }
    
    // Default to goblin if not found
    return &ENEMY_DATABASE[0];
}

// Initialize an enemy with proper stats for level
void initEnemyByType(Enemy* enemy, EnemyType type, int level) {
    // Get base enemy data
    EnemyData* data = getEnemyData(type);
    
    // Copy basic information
    strcpy(enemy->name, data->name);
    
    // Scale stats based on level
    enemy->maxHp = data->baseHp + (level * 5);
    enemy->hp = enemy->maxHp;
    enemy->attack = data->baseAttack + (level * 2);
    enemy->defense = data->baseDefense + (level);
    enemy->speed = data->baseSpeed + (level / 2);
    
    // Reward scales with level
    enemy->rewardCoins = data->baseReward + (level * 3);
    
    // Set dropped item
    if (getRandomInt(100) < 30) {
        // Rare drop (30% chance)
        initItem(&enemy->droppedItem, data->rareDrop);
    } else {
        // Common drop
        initItem(&enemy->droppedItem, data->commonDrop);
    }
    
    // Set drop rate
    enemy->dropRate = data->dropRate;
    
    // Bosses get bonus stats
    if (data->bossFlag == ENEMY_BOSS) {
        enemy->maxHp *= 1.5;
        enemy->hp = enemy->maxHp;
        enemy->attack *= 1.2;
        enemy->defense *= 1.2;
        enemy->rewardCoins *= 2;
        enemy->dropRate += 20; // Bosses have better drop rates
    }
}

// Scale enemy stats based on level
void scaleEnemyStats(Enemy* enemy, int level) {
    // Simple linear scaling based on level
    float multiplier = 1.0 + (level * 0.1);
    
    enemy->maxHp = (int)(enemy->maxHp * multiplier);
    enemy->hp = enemy->maxHp;
    enemy->attack = (int)(enemy->attack * multiplier);
    enemy->defense = (int)(enemy->defense * multiplier);
    enemy->speed = (int)(enemy->speed * multiplier);
    enemy->rewardCoins = (int)(enemy->rewardCoins * multiplier);
}

// Get random enemy type appropriate for the map
EnemyType getRandomEnemyType(int minLevel, int maxLevel) {
    // For early levels (1-5), only use weaker enemies
    if (maxLevel <= 5) {
        // Choose among basic enemies: goblin, skeleton, wolf
        switch (getRandomInt(3)) {
            case 0: return ENEMY_GOBLIN;
            case 1: return ENEMY_SKELETON;
            case 2: return ENEMY_WOLF;
        }
    }
    
    // For mid levels (6-10), add more enemy types
    if (maxLevel <= 10) {
        switch (getRandomInt(6)) {
            case 0: return ENEMY_GOBLIN;
            case 1: return ENEMY_SKELETON;
            case 2: return ENEMY_WOLF;
            case 3: return ENEMY_ORC;
            case 4: return ENEMY_ZOMBIE;
            case 5: return ENEMY_BANDIT;
        }
    }
    
    // For high levels (11-15), add tough enemies
    if (maxLevel <= 15) {
        switch (getRandomInt(8)) {
            case 0: return ENEMY_ORC;
            case 1: return ENEMY_ZOMBIE;
            case 2: return ENEMY_BANDIT;
            case 3: return ENEMY_TROLL;
            case 4: return ENEMY_GHOST;
            case 5: return ENEMY_WOLF;
            case 6: return ENEMY_GOBLIN;
            case 7: return ENEMY_SKELETON;
        }
    }
    
    // For end game (16+), include all enemy types with rare boss encounters
    int roll = getRandomInt(100);
    
    // 10% chance for a boss
    if (roll < 10) {
        // Choose between dragon and dark knight
        return (getRandomInt(2) == 0) ? ENEMY_DRAGON : ENEMY_DARK_KNIGHT;
    }
    
    // 90% chance for regular enemies
    switch (getRandomInt(8)) {
        case 0: return ENEMY_ORC;
        case 1: return ENEMY_ZOMBIE;
        case 2: return ENEMY_BANDIT;
        case 3: return ENEMY_TROLL;
        case 4: return ENEMY_GHOST;
        case 5: return ENEMY_WOLF;
        case 6: return ENEMY_GOBLIN;
        case 7: return ENEMY_SKELETON;
        default: return ENEMY_ORC;
    }
}
