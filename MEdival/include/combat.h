#ifndef COMBAT_H
#define COMBAT_H

#include "characters.h"

// Combat Actions
typedef enum {
    ACTION_ATTACK,
    ACTION_USE_ITEM,
    ACTION_DEFEND,
    ACTION_RUN
} CombatAction;

// Combat state machine
typedef enum {
    COMBAT_START,
    COMBAT_PLAYER_TURN,
    COMBAT_ENEMY_TURN,
    COMBAT_VICTORY,
    COMBAT_DEFEAT,
    COMBAT_FLED
} CombatState;

// Combat context structure
typedef struct {
    Player* player;
    Enemy* enemy;
    CombatState state;
    int playerDefending;
    int turnCount;
    int selectedAction;
    int selectedItemIndex;
    int messageTimer;
    char battleMessage[50];
} CombatContext;

// Initialize combat with player and enemy
void initCombat(CombatContext* context, Player* player, Enemy* enemy);

// Update combat state
void updateCombat(CombatContext* context);

// Player performs an action
void performPlayerAction(CombatContext* context, CombatAction action);

// Enemy performs an action
void performEnemyAction(CombatContext* context);

// Calculate damage with attacker and defender stats
int calculateDamage(int attackStat, int defenseStat, int weaponDamage);

// Check if combat has ended
int isCombatOver(CombatContext* context);

// Apply rewards to player after victory
void applyVictoryRewards(CombatContext* context);

// Apply weapon training points from combat
void applyWeaponTraining(Player* player, int points);

// Handle running away from combat
int attemptRun(CombatContext* context);

// Display combat UI
void renderCombatUI(CombatContext* context);

#endif // COMBAT_H
