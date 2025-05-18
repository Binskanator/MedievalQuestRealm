#include "../include/items.h"
#include "../include/characters.h"
#include <string.h>

// Item database
Item ITEM_DATABASE[] = {
    {ITEM_POTION, "Potion", 20, 30, 1, "Restore 20 HP"},
    {ITEM_HI_POTION, "Hi-Potion", 50, 80, 1, "Restore 50 HP"},
    {ITEM_SUPER_POTION, "Super Potion", 100, 150, 1, "Restore 100 HP"},
    {ITEM_REPAIR_KIT, "Repair Kit", 30, 60, 1, "Repair armor by 30 points"},
    {ITEM_ATTACK_BOOST, "Attack Boost", 5, 100, 1, "Temporarily boost Attack by 5"},
    {ITEM_DEFENSE_BOOST, "Defense Boost", 5, 100, 1, "Temporarily boost Defense by 5"},
    {ITEM_TRAINING_MANUAL, "Training Manual", 5, 200, 1, "Add 5 training points to weapon"}
};

const int ITEM_DATABASE_SIZE = sizeof(ITEM_DATABASE) / sizeof(Item);

// Initialize an item
void initItem(Item* item, ItemType type) {
    // Find the item in the database
    for (int i = 0; i < ITEM_DATABASE_SIZE; i++) {
        if (ITEM_DATABASE[i].type == type) {
            *item = ITEM_DATABASE[i];
            return;
        }
    }
    
    // Default to NONE if not found
    item->type = ITEM_NONE;
    strcpy(item->name, "None");
    item->value = 0;
    item->cost = 0;
    item->count = 0;
    strcpy(item->description, "No effect");
}

// Get item name by type
const char* getItemName(ItemType type) {
    for (int i = 0; i < ITEM_DATABASE_SIZE; i++) {
        if (ITEM_DATABASE[i].type == type) {
            return ITEM_DATABASE[i].name;
        }
    }
    return "Unknown";
}

// Get item description by type
const char* getItemDescription(ItemType type) {
    for (int i = 0; i < ITEM_DATABASE_SIZE; i++) {
        if (ITEM_DATABASE[i].type == type) {
            return ITEM_DATABASE[i].description;
        }
    }
    return "No effect";
}

// Get item value (healing amount, stat boost, etc.)
int getItemValue(ItemType type) {
    for (int i = 0; i < ITEM_DATABASE_SIZE; i++) {
        if (ITEM_DATABASE[i].type == type) {
            return ITEM_DATABASE[i].value;
        }
    }
    return 0;
}

// Get item base cost
int getItemCost(ItemType type) {
    for (int i = 0; i < ITEM_DATABASE_SIZE; i++) {
        if (ITEM_DATABASE[i].type == type) {
            return ITEM_DATABASE[i].cost;
        }
    }
    return 0;
}

// Apply item effect to player
void applyItemEffect(Item* item, void* target) {
    Player* player = (Player*)target;
    
    switch (item->type) {
        case ITEM_POTION:
        case ITEM_HI_POTION:
        case ITEM_SUPER_POTION:
            // Healing items
            healPlayer(player, item->value);
            break;
            
        case ITEM_REPAIR_KIT:
            // Repair armor
            repairArmor(player, item->value);
            break;
            
        case ITEM_ATTACK_BOOST:
            // Temporarily boost attack (for the next battle)
            player->stats.attack += item->value;
            break;
            
        case ITEM_DEFENSE_BOOST:
            // Temporarily boost defense (for the next battle)
            player->stats.defense += item->value;
            break;
            
        case ITEM_TRAINING_MANUAL:
            // Add training points to weapon
            player->weapon.trainingPoints += item->value;
            
            // Check if weapon can evolve
            if (canWeaponEvolve(&player->weapon)) {
                evolveWeapon(&player->weapon);
            }
            break;
            
        default:
            // No effect
            break;
    }
}
