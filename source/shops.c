#include "../include/shops.h"
#include "../include/input.h"
#include "../include/text.h"
#include "../include/graphics.h"
#include <string.h>

// Initialize shop with appropriate inventory
void initShop(Shop* shop, ShopType type) {
    shop->type = type;
    shop->itemCount = 0;
    
    // Set shop name based on type
    switch (type) {
        case SHOP_GENERAL:
            strcpy(shop->name, "General Store");
            break;
        case SHOP_WEAPONS:
            strcpy(shop->name, "Weapons Shop");
            break;
        case SHOP_ARMOR:
            strcpy(shop->name, "Armor Shop");
            break;
        case SHOP_POTIONS:
            strcpy(shop->name, "Potion Shop");
            break;
    }
    
    // Fill inventory based on shop type
    switch (type) {
        case SHOP_GENERAL:
            // Add general items
            initItem(&shop->inventory[shop->itemCount++], ITEM_POTION);
            initItem(&shop->inventory[shop->itemCount++], ITEM_REPAIR_KIT);
            break;
            
        case SHOP_WEAPONS:
            // Add weapon-related items
            initItem(&shop->inventory[shop->itemCount++], ITEM_TRAINING_MANUAL);
            initItem(&shop->inventory[shop->itemCount++], ITEM_ATTACK_BOOST);
            break;
            
        case SHOP_ARMOR:
            // Add armor-related items
            initItem(&shop->inventory[shop->itemCount++], ITEM_REPAIR_KIT);
            initItem(&shop->inventory[shop->itemCount++], ITEM_DEFENSE_BOOST);
            break;
            
        case SHOP_POTIONS:
            // Add healing items
            initItem(&shop->inventory[shop->itemCount++], ITEM_POTION);
            initItem(&shop->inventory[shop->itemCount++], ITEM_HI_POTION);
            initItem(&shop->inventory[shop->itemCount++], ITEM_SUPER_POTION);
            break;
    }
}

// Initialize shop context for UI
void initShop(ShopContext* context, Shop* shop, Player* player) {
    context->shop = shop;
    context->player = player;
    context->state = SHOP_STATE_BROWSING;
    context->selectedItem = 0;
    context->quantity = 1;
    context->messageTimer = 0;
    
    strcpy(context->message, "Welcome to the shop!");
}

// Buy item from shop
int buyItem(ShopContext* context, int itemIndex, int quantity) {
    // Check if item index is valid
    if (itemIndex < 0 || itemIndex >= context->shop->itemCount) {
        strcpy(context->message, "Invalid item selection!");
        context->messageTimer = 60;
        return 0;
    }
    
    // Get the item
    Item* item = &context->shop->inventory[itemIndex];
    
    // Calculate total cost
    int totalCost = getShopPrice(item, context->player, 1) * quantity;
    
    // Check if player has enough money
    if (context->player->coins < totalCost) {
        strcpy(context->message, "Not enough coins!");
        context->messageTimer = 60;
        return 0;
    }
    
    // Create a copy of the item to add to inventory
    Item boughtItem = *item;
    boughtItem.count = quantity;
    
    // Add to player's inventory
    addItemToInventory(context->player, boughtItem);
    
    // Deduct coins
    context->player->coins -= totalCost;
    
    // Set success message
    sprintf(context->message, "Bought %d %s for %d coins.", 
           quantity, item->name, totalCost);
    context->messageTimer = 60;
    
    return 1;
}

// Sell item to shop
int sellItem(ShopContext* context, int playerItemIndex, int quantity) {
    // Check if item index is valid
    if (playerItemIndex < 0 || playerItemIndex >= context->player->inventoryCount) {
        strcpy(context->message, "Invalid item selection!");
        context->messageTimer = 60;
        return 0;
    }
    
    // Get the item
    Item* item = &context->player->inventory[playerItemIndex];
    
    // Check if player has enough of the item
    if (item->count < quantity) {
        strcpy(context->message, "Not enough items!");
        context->messageTimer = 60;
        return 0;
    }
    
    // Calculate total value
    int totalValue = getShopPrice(item, context->player, 0) * quantity;
    
    // Remove items from inventory
    for (int i = 0; i < quantity; i++) {
        removeItemFromInventory(context->player, playerItemIndex);
    }
    
    // Add coins
    context->player->coins += totalValue;
    
    // Set success message
    sprintf(context->message, "Sold %d %s for %d coins.", 
           quantity, item->name, totalValue);
    context->messageTimer = 60;
    
    return 1;
}

// Update shop state
void updateShop(ShopContext* context) {
    // Handle message timer
    if (context->messageTimer > 0) {
        context->messageTimer--;
        
        // Return to browsing when message times out
        if (context->messageTimer == 0 && 
            (context->state == SHOP_STATE_MESSAGE)) {
            context->state = SHOP_STATE_BROWSING;
        }
    }
    
    // State machine for shop interaction
    switch (context->state) {
        case SHOP_STATE_BROWSING:
            // Navigation
            if (keyPressed(&inputState, KEY_UP)) {
                if (context->selectedItem > 0) {
                    context->selectedItem--;
                }
            } else if (keyPressed(&inputState, KEY_DOWN)) {
                if (context->selectedItem < context->shop->itemCount - 1) {
                    context->selectedItem++;
                }
            }
            
            // Select an item to buy
            if (keyPressed(&inputState, KEY_A)) {
                context->state = SHOP_STATE_BUYING;
                context->quantity = 1;
            }
            
            // Switch to selling
            if (keyPressed(&inputState, KEY_R)) {
                context->state = SHOP_STATE_SELLING;
                context->selectedItem = 0;
                context->quantity = 1;
            }
            break;
            
        case SHOP_STATE_BUYING:
            // Adjust quantity
            if (keyPressed(&inputState, KEY_LEFT)) {
                if (context->quantity > 1) {
                    context->quantity--;
                }
            } else if (keyPressed(&inputState, KEY_RIGHT)) {
                if (context->quantity < 99) {
                    context->quantity++;
                }
            }
            
            // Confirm purchase
            if (keyPressed(&inputState, KEY_A)) {
                context->state = SHOP_STATE_CONFIRM;
            }
            
            // Cancel
            if (keyPressed(&inputState, KEY_B)) {
                context->state = SHOP_STATE_BROWSING;
            }
            break;
            
        case SHOP_STATE_SELLING:
            // Navigation
            if (keyPressed(&inputState, KEY_UP)) {
                if (context->selectedItem > 0) {
                    context->selectedItem--;
                }
            } else if (keyPressed(&inputState, KEY_DOWN)) {
                if (context->selectedItem < context->player->inventoryCount - 1) {
                    context->selectedItem++;
                }
            }
            
            // Adjust quantity
            if (keyPressed(&inputState, KEY_LEFT)) {
                if (context->quantity > 1) {
                    context->quantity--;
                }
            } else if (keyPressed(&inputState, KEY_RIGHT)) {
                if (context->quantity < context->player->inventory[context->selectedItem].count) {
                    context->quantity++;
                }
            }
            
            // Confirm sale
            if (keyPressed(&inputState, KEY_A) && context->player->inventoryCount > 0) {
                context->state = SHOP_STATE_CONFIRM;
            }
            
            // Cancel
            if (keyPressed(&inputState, KEY_B)) {
                context->state = SHOP_STATE_BROWSING;
            }
            
            // Switch to buying
            if (keyPressed(&inputState, KEY_R)) {
                context->state = SHOP_STATE_BROWSING;
                context->selectedItem = 0;
                context->quantity = 1;
            }
            break;
            
        case SHOP_STATE_CONFIRM:
            // Confirm
            if (keyPressed(&inputState, KEY_A)) {
                if (context->state == SHOP_STATE_BUYING || 
                    context->state == SHOP_STATE_CONFIRM && 
                    context->selectedItem < context->shop->itemCount) {
                    // Buy the item
                    buyItem(context, context->selectedItem, context->quantity);
                    context->state = SHOP_STATE_MESSAGE;
                } else {
                    // Sell the item
                    sellItem(context, context->selectedItem, context->quantity);
                    context->state = SHOP_STATE_MESSAGE;
                    
                    // Adjust selected item if we've sold everything
                    if (context->player->inventoryCount <= context->selectedItem) {
                        context->selectedItem = context->player->inventoryCount - 1;
                        if (context->selectedItem < 0) context->selectedItem = 0;
                    }
                }
            }
            
            // Cancel
            if (keyPressed(&inputState, KEY_B)) {
                if (context->selectedItem < context->shop->itemCount) {
                    context->state = SHOP_STATE_BUYING;
                } else {
                    context->state = SHOP_STATE_SELLING;
                }
            }
            break;
            
        case SHOP_STATE_MESSAGE:
            // Just wait for message timer to expire or button press
            if (keyPressed(&inputState, KEY_A) || keyPressed(&inputState, KEY_B)) {
                context->messageTimer = 0;
                context->state = SHOP_STATE_BROWSING;
            }
            break;
    }
}

// Render shop UI
void renderShopUI(ShopContext* context) {
    // Clear screen
    fillScreen(BLACK);
    
    // Draw shop name
    printTextAligned(SCREEN_WIDTH / 2, 10, context->shop->name, WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
    
    // Draw player coins
    drawHollowRect(160, 10, 70, 15, WHITE);
    printText(165, 15, "Coins:", WHITE);
    printNumber(210, 15, context->player->coins, YELLOW);
    
    // Draw message box
    drawHollowRect(10, 140, 220, 20, WHITE);
    printText(20, 145, context->message, WHITE);
    
    // Draw UI based on current state
    switch (context->state) {
        case SHOP_STATE_BROWSING:
        case SHOP_STATE_BUYING:
        case SHOP_STATE_CONFIRM:
            if (context->selectedItem < context->shop->itemCount) {
                // Draw shop inventory
                printTextAligned(SCREEN_WIDTH / 2, 30, "ITEMS FOR SALE", WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
                
                for (int i = 0; i < context->shop->itemCount; i++) {
                    Color itemColor = (context->selectedItem == i) ? YELLOW : WHITE;
                    
                    printText(30, 50 + i * 15, context->shop->inventory[i].name, itemColor);
                    printText(150, 50 + i * 15, "Cost:", itemColor);
                    printNumber(190, 50 + i * 15, 
                              getShopPrice(&context->shop->inventory[i], context->player, 1), 
                              itemColor);
                    
                    // Draw cursor
                    if (context->selectedItem == i) {
                        printText(15, 50 + i * 15, ">", YELLOW);
                    }
                }
                
                // Draw selected item details
                if (context->shop->itemCount > 0) {
                    drawHollowRect(20, 110, 200, 20, WHITE);
                    printText(30, 120, context->shop->inventory[context->selectedItem].description, WHITE);
                }
                
                // Draw quantity selection in buying mode
                if (context->state == SHOP_STATE_BUYING || 
                    (context->state == SHOP_STATE_CONFIRM && 
                     context->selectedItem < context->shop->itemCount)) {
                    printText(30, 170, "Quantity:", WHITE);
                    printNumber(90, 170, context->quantity, YELLOW);
                    
                    // Show total cost
                    printText(120, 170, "Total:", WHITE);
                    int totalCost = getShopPrice(&context->shop->inventory[context->selectedItem], 
                                              context->player, 1) * context->quantity;
                    printNumber(170, 170, totalCost, YELLOW);
                    
                    // Show confirmation message
                    if (context->state == SHOP_STATE_CONFIRM) {
                        printTextAligned(SCREEN_WIDTH / 2, 190, "Press A to confirm purchase", 
                                    GREEN, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
                    }
                }
            }
            break;
            
        case SHOP_STATE_SELLING:
            // Draw player inventory
            printTextAligned(SCREEN_WIDTH / 2, 30, "YOUR ITEMS", WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            
            if (context->player->inventoryCount > 0) {
                for (int i = 0; i < context->player->inventoryCount; i++) {
                    Color itemColor = (context->selectedItem == i) ? YELLOW : WHITE;
                    
                    printText(30, 50 + i * 15, context->player->inventory[i].name, itemColor);
                    printText(130, 50 + i * 15, "x", itemColor);
                    printNumber(140, 50 + i * 15, context->player->inventory[i].count, itemColor);
                    printText(160, 50 + i * 15, "Value:", itemColor);
                    printNumber(200, 50 + i * 15, 
                              getShopPrice(&context->player->inventory[i], context->player, 0), 
                              itemColor);
                    
                    // Draw cursor
                    if (context->selectedItem == i) {
                        printText(15, 50 + i * 15, ">", YELLOW);
                    }
                }
                
                // Draw quantity selection
                printText(30, 170, "Quantity:", WHITE);
                printNumber(90, 170, context->quantity, YELLOW);
                
                // Show total value
                printText(120, 170, "Total:", WHITE);
                int totalValue = getShopPrice(&context->player->inventory[context->selectedItem], 
                                          context->player, 0) * context->quantity;
                printNumber(170, 170, totalValue, YELLOW);
                
                // Show confirmation message
                if (context->state == SHOP_STATE_CONFIRM) {
                    printTextAligned(SCREEN_WIDTH / 2, 190, "Press A to confirm sale", 
                                GREEN, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
                }
            } else {
                printTextAligned(SCREEN_WIDTH / 2, 80, "Inventory is empty", 
                            GRAY, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            }
            break;
            
        case SHOP_STATE_MESSAGE:
            // Just showing the message
            break;
    }
    
    // Draw instructions
    if (context->state == SHOP_STATE_BROWSING) {
        printText(10, 190, "A: Buy", BLUE);
        printText(80, 190, "R: Sell", BLUE);
        printText(150, 190, "B: Exit", BLUE);
    } else if (context->state == SHOP_STATE_SELLING) {
        printText(10, 190, "A: Sell", BLUE);
        printText(80, 190, "R: Buy", BLUE);
        printText(150, 190, "B: Back", BLUE);
    } else if (context->state == SHOP_STATE_BUYING) {
        printText(10, 190, "A: Confirm", BLUE);
        printText(90, 190, "B: Back", BLUE);
    }
}

// Get shop item price based on type and player's stats
int getShopPrice(Item* item, Player* player, int buying) {
    if (buying) {
        // Price for buying
        return item->cost;
    } else {
        // Price for selling (50% of buying price)
        return item->cost / 2;
    }
}
