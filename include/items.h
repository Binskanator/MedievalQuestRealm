#ifndef ITEMS_H
#define ITEMS_H

// Item types
typedef enum {
    ITEM_NONE,
    ITEM_POTION,
    ITEM_HI_POTION,
    ITEM_SUPER_POTION,
    ITEM_REPAIR_KIT,
    ITEM_ATTACK_BOOST,
    ITEM_DEFENSE_BOOST,
    ITEM_TRAINING_MANUAL
} ItemType;

// Item structure
typedef struct {
    ItemType type;
    char name[20];
    int value;
    int cost;
    int count;
    char description[50];
} Item;

// Initialize an item
void initItem(Item* item, ItemType type);

// Get item name by type
const char* getItemName(ItemType type);

// Get item description by type
const char* getItemDescription(ItemType type);

// Get item value (healing amount, stat boost, etc.)
int getItemValue(ItemType type);

// Get item base cost
int getItemCost(ItemType type);

// Apply item effect to player
void applyItemEffect(Item* item, void* target);

// Item database
extern Item ITEM_DATABASE[];
extern const int ITEM_DATABASE_SIZE;

#endif // ITEMS_H
