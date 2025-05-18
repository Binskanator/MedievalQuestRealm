#ifndef SHOPS_H
#define SHOPS_H

#include "items.h"
#include "characters.h"

// Shop types
typedef enum {
    SHOP_GENERAL,
    SHOP_WEAPONS,
    SHOP_ARMOR,
    SHOP_POTIONS
} ShopType;

// Shop inventory
typedef struct {
    ShopType type;
    char name[20];
    Item inventory[10];
    int itemCount;
} Shop;

// Shop state
typedef enum {
    SHOP_STATE_BROWSING,
    SHOP_STATE_BUYING,
    SHOP_STATE_SELLING,
    SHOP_STATE_CONFIRM,
    SHOP_STATE_MESSAGE
} ShopState;

// Shop context for UI
typedef struct {
    Shop* shop;
    Player* player;
    ShopState state;
    int selectedItem;
    int quantity;
    char message[50];
    int messageTimer;
} ShopContext;

// Initialize shop with appropriate inventory
void initShop(Shop* shop, ShopType type);

// Buy item from shop
int buyItem(ShopContext* context, int itemIndex, int quantity);

// Sell item to shop
int sellItem(ShopContext* context, int playerItemIndex, int quantity);

// Update shop state
void updateShop(ShopContext* context);

// Render shop UI
void renderShopUI(ShopContext* context);

// Get shop item price based on type and player's stats
int getShopPrice(Item* item, Player* player, int buying);

#endif // SHOPS_H
