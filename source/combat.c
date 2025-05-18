
#include "../include/combat.h"
#include "../include/input.h"
#include "../include/graphics.h"
#include "../include/text.h"
#include <stdlib.h>

InputState inputState;

#include "../include/combat.h"
#include "../include/game.h"
#include <string.h>
#include <stdio.h>

// Initialize combat with player and enemy
void initCombat(CombatContext* context, Player* player, Enemy* enemy) {
    context->player = player;
    context->enemy = enemy;
    context->state = COMBAT_START;
    context->playerDefending = 0;
    context->turnCount = 0;
    context->selectedAction = ACTION_ATTACK;
    context->selectedItemIndex = -1;
    context->messageTimer = 0;
    
    // Set initial battle message
    sprintf(context->battleMessage, "A %s appeared!", enemy->name);
}

// Update combat state
void updateCombat(CombatContext* context) {
    // Handle message timer
    if (context->messageTimer > 0) {
        context->messageTimer--;
        return;
    }
    
    // State machine for combat
    switch (context->state) {
        case COMBAT_START:
            // Transition to player turn
            context->state = COMBAT_PLAYER_TURN;
            break;
            
        case COMBAT_PLAYER_TURN:
            // Handle player input
            if (keyPressed(&inputState, KEY_UP)) {
                context->selectedAction = (context->selectedAction - 1 + 4) % 4;
            } else if (keyPressed(&inputState, KEY_DOWN)) {
                context->selectedAction = (context->selectedAction + 1) % 4;
            }
            
            // Execute selected action
            if (keyPressed(&inputState, KEY_A)) {
                performPlayerAction(context, context->selectedAction);
                
                // Reset defending flag at end of turn
                context->playerDefending = 0;
                
                // Move to enemy turn if combat continues
                if (context->enemy->hp > 0) {
                    context->state = COMBAT_ENEMY_TURN;
                } else {
                    context->state = COMBAT_VICTORY;
                    sprintf(context->battleMessage, "You defeated the %s!", context->enemy->name);
                    context->messageTimer = 60; // 1 second message display
                }
            }
            break;
            
        case COMBAT_ENEMY_TURN:
            // Enemy performs action
            performEnemyAction(context);
            
            // Increment turn counter
            context->turnCount++;
            
            // Check player status
            if (context->player->stats.hp <= 0) {
                context->state = COMBAT_DEFEAT;
                strcpy(context->battleMessage, "You were defeated!");
                context->messageTimer = 60;
            } else {
                context->state = COMBAT_PLAYER_TURN;
            }
            break;
            
        case COMBAT_VICTORY:
            // Apply rewards on victory
            if (context->messageTimer == 0) {
                applyVictoryRewards(context);
                
                // Check for item drop
                if (context->enemy->dropRate > 0 && getRandomInt(100) < context->enemy->dropRate) {
                    addItemToInventory(context->player, context->enemy->droppedItem);
                    sprintf(context->battleMessage, "Got %s!", context->enemy->droppedItem.name);
                    context->messageTimer = 60;
                }
            }
            break;
            
        case COMBAT_DEFEAT:
            // Nothing to do, game over is handled in main loop
            break;
            
        case COMBAT_FLED:
            // Nothing to do, return to world is handled in main loop
            break;
    }
}

// Player performs an action
void performPlayerAction(CombatContext* context, CombatAction action) {
    switch (action) {
        case ACTION_ATTACK:
            {
                // Calculate damage
                int damage = calculateDamage(context->player->stats.attack, 
                                            context->enemy->defense, 
                                            context->player->weapon.baseDamage);
                
                // Apply damage to enemy
                context->enemy->hp -= damage;
                if (context->enemy->hp < 0) context->enemy->hp = 0;
                
                // Set message
                sprintf(context->battleMessage, "You hit for %d damage!", damage);
                context->messageTimer = 30;
                
                // Reduce weapon durability
                context->player->weapon.durability--;
                if (context->player->weapon.durability < 0) {
                    context->player->weapon.durability = 0;
                    context->player->weapon.baseDamage /= 2; // Damaged weapon does less damage
                }
            }
            break;
            
        case ACTION_USE_ITEM:
            {
                // Show inventory for item selection
                if (context->selectedItemIndex == -1) {
                    // Set message prompting item selection
                    strcpy(context->battleMessage, "Select an item to use...");
                    context->messageTimer = 1;
                    return;
                }
                
                // Use the selected item
                if (context->selectedItemIndex >= 0 && 
                    context->selectedItemIndex < context->player->inventoryCount) {
                    
                    Item* item = &context->player->inventory[context->selectedItemIndex];
                    sprintf(context->battleMessage, "Used %s!", item->name);
                    
                    // Apply item effect
                    useItem(context->player, context->selectedItemIndex);
                    
                    context->messageTimer = 30;
                    context->selectedItemIndex = -1;
                }
            }
            break;
            
        case ACTION_DEFEND:
            {
                // Increase defense for enemy's next attack
                context->playerDefending = 1;
                
                strcpy(context->battleMessage, "Defending...");
                context->messageTimer = 30;
            }
            break;
            
        case ACTION_RUN:
            {
                // Attempt to run
                if (attemptRun(context)) {
                    context->state = COMBAT_FLED;
                    strcpy(context->battleMessage, "Got away safely!");
                } else {
                    strcpy(context->battleMessage, "Couldn't escape!");
                }
                context->messageTimer = 30;
            }
            break;
    }
}

// Enemy performs an action
void performEnemyAction(CombatContext* context) {
    // Simple AI - always attack
    
    // Calculate damage
    int defenseBonus = context->playerDefending ? context->player->stats.defense / 2 : 0;
    int damage = calculateDamage(context->enemy->attack, 
                               context->player->stats.defense + defenseBonus, 
                               0);
    
    // Ensure minimum damage
    if (damage < 1) damage = 1;
    
    // Apply damage to player
    context->player->stats.hp -= damage;
    if (context->player->stats.hp < 0) context->player->stats.hp = 0;
    
    // Reduce armor durability
    context->player->armor.durability--;
    if (context->player->armor.durability <= 0) {
        context->player->armor.durability = 0;
        context->player->armor.defense = 0; // Broken armor provides no defense
    }
    
    // Set message
    sprintf(context->battleMessage, "%s hit you for %d damage!", 
           context->enemy->name, damage);
    context->messageTimer = 30;
}

// Calculate damage with attacker and defender stats
int calculateDamage(int attackStat, int defenseStat, int weaponDamage) {
    int baseDamage = attackStat + weaponDamage - defenseStat / 2;
    
    // Apply random variance (80% - 120%)
    int variance = 80 + getRandomInt(41); // 80-120
    baseDamage = (baseDamage * variance) / 100;
    
    // Ensure minimum damage
    if (baseDamage < 1) baseDamage = 1;
    
    return baseDamage;
}

// Check if combat has ended
int isCombatOver(CombatContext* context) {
    return (context->state == COMBAT_VICTORY || 
            context->state == COMBAT_DEFEAT || 
            context->state == COMBAT_FLED);
}

// Apply rewards to player after victory
void applyVictoryRewards(CombatContext* context) {
    // Award coins
    context->player->coins += context->enemy->rewardCoins;
    
    // Award experience (represented as defeated enemies)
    context->player->defeatedEnemies++;
}

// Apply weapon training points from combat
void applyWeaponTraining(Player* player, int points) {
    player->weapon.trainingPoints += points;
}

// Handle running away from combat
int attemptRun(CombatContext* context) {
    // Base chance of 50%
    int escapeChance = 50;
    
    // Adjust based on speed difference
    escapeChance += (context->player->stats.speed - context->enemy->speed) * 5;
    
    // Cap escape chance
    if (escapeChance < 20) escapeChance = 20;
    if (escapeChance > 90) escapeChance = 90;
    
    // Roll for escape
    return (getRandomInt(100) < escapeChance);
}

// Render combat UI
void renderCombatUI(CombatContext* context) {
    // Clear screen
    fillScreen(BLACK);
    
    // Draw enemy section
    printText(20, 20, context->enemy->name, RED);
    printText(20, 35, "HP:", WHITE);
    printNumber(45, 35, context->enemy->hp, WHITE);
    printText(65, 35, "/", WHITE);
    printNumber(75, 35, context->enemy->maxHp, WHITE);
    
    // Draw player section
    printText(20, 110, context->player->name, BLUE);
    printText(20, 125, "HP:", WHITE);
    printNumber(45, 125, context->player->stats.hp, WHITE);
    printText(65, 125, "/", WHITE);
    printNumber(75, 125, context->player->stats.maxHp, WHITE);
    
    // Draw battle message
    drawHollowRect(10, 75, 220, 25, WHITE);
    printText(20, 85, context->battleMessage, WHITE);
    
    // Draw action menu (only during player turn)
    if (context->state == COMBAT_PLAYER_TURN) {
        drawHollowRect(130, 110, 100, 40, WHITE);
        
        Color attackColor = (context->selectedAction == ACTION_ATTACK) ? YELLOW : WHITE;
        Color itemColor = (context->selectedAction == ACTION_USE_ITEM) ? YELLOW : WHITE;
        Color defendColor = (context->selectedAction == ACTION_DEFEND) ? YELLOW : WHITE;
        Color runColor = (context->selectedAction == ACTION_RUN) ? YELLOW : WHITE;
        
        printText(150, 115, "Attack", attackColor);
        printText(150, 125, "Item", itemColor);
        printText(150, 135, "Defend", defendColor);
        printText(150, 145, "Run", runColor);
        
        // Draw cursor
        printText(140, 115 + context->selectedAction * 10, ">", YELLOW);
    }
}
