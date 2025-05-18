#ifndef ENEMIES_H
#define ENEMIES_H

#include "characters.h"

// Enemy types
typedef enum {
    ENEMY_GOBLIN,
    ENEMY_ORC,
    ENEMY_SKELETON,
    ENEMY_ZOMBIE,
    ENEMY_BANDIT,
    ENEMY_WOLF,
    ENEMY_TROLL,
    ENEMY_GHOST,
    ENEMY_DRAGON,
    ENEMY_DARK_KNIGHT
} EnemyType;

// Boss flag
typedef enum {
    ENEMY_NORMAL,
    ENEMY_BOSS
} EnemyRank;

// Enemy data structure for database
typedef struct {
    EnemyType type;
    char name[20];
    int baseHp;
    int baseAttack;
    int baseDefense;
    int baseSpeed;
    int baseReward;
    ItemType commonDrop;
    ItemType rareDrop;
    int dropRate;
    int bossFlag;
} EnemyData;

// Get enemy data by type
EnemyData* getEnemyData(EnemyType type);

// Initialize an enemy with proper stats for level
void initEnemyByType(Enemy* enemy, EnemyType type, int level);

// Scale enemy stats based on level
void scaleEnemyStats(Enemy* enemy, int level);

// Get random enemy type appropriate for the map
EnemyType getRandomEnemyType(int minLevel, int maxLevel);

// Enemy database
extern EnemyData ENEMY_DATABASE[];
extern const int ENEMY_DATABASE_SIZE;

#endif // ENEMIES_H
