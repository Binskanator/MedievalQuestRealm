#include "../include/training.h"
#include "../include/input.h"
#include "../include/graphics.h"
#include "../include/text.h"
#include "../include/game.h"
#include <string.h>
#include <stdio.h>

// Initialize training context
void initTraining(TrainingContext* context, Player* player) {
    context->player = player;
    context->selectedType = TRAINING_STRENGTH;
    context->selectedDifficulty = DIFFICULTY_EASY;
    context->trainingInProgress = 0;
    context->trainingSuccess = 0;
    context->trainingPoints = 0;
    context->minigameTimer = 0;
    context->minigameState = 0;
    context->messageTimer = 60;
    
    strcpy(context->message, "Welcome to the Training Grounds!");
}

// Start training minigame
void startTraining(TrainingContext* context) {
    context->trainingInProgress = 1;
    context->minigameTimer = 0;
    context->minigameState = 0;
    
    // Set initial message
    switch (context->selectedType) {
        case TRAINING_STRENGTH:
            strcpy(context->message, "Press A rapidly to build strength!");
            break;
        case TRAINING_TECHNIQUE:
            strcpy(context->message, "Press buttons in the shown sequence!");
            break;
        case TRAINING_SPEED:
            strcpy(context->message, "Press A when the bar is in the center!");
            break;
    }
    
    context->messageTimer = 60;
}

// Update training minigame state
void updateTraining(TrainingContext* context) {
    // Handle message timer
    if (context->messageTimer > 0) {
        context->messageTimer--;
    }
    
    // If not in a training session, handle menu navigation
    if (!context->trainingInProgress) {
        // Type selection
        if (keyPressed(&inputState, KEY_LEFT)) {
            context->selectedType = (context->selectedType - 1 + 3) % 3;
        } else if (keyPressed(&inputState, KEY_RIGHT)) {
            context->selectedType = (context->selectedType + 1) % 3;
        }
        
        // Difficulty selection
        if (keyPressed(&inputState, KEY_UP)) {
            context->selectedDifficulty = (context->selectedDifficulty - 1 + 3) % 3;
        } else if (keyPressed(&inputState, KEY_DOWN)) {
            context->selectedDifficulty = (context->selectedDifficulty + 1) % 3;
        }
        
        // Start training
        if (keyPressed(&inputState, KEY_A)) {
            startTraining(context);
        }
        
        return;
    }
    
    // Update minigame based on type
    switch (context->selectedType) {
        case TRAINING_STRENGTH:
            // Strength training - button mashing
            if (keyPressed(&inputState, KEY_A)) {
                context->minigameState++;
            }
            
            // Timer runs down
            context->minigameTimer++;
            
            // Check if time's up (3 seconds)
            if (context->minigameTimer >= 180) {
                // Calculate success based on button presses
                int threshold;
                switch (context->selectedDifficulty) {
                    case DIFFICULTY_EASY:
                        threshold = 15;
                        break;
                    case DIFFICULTY_MEDIUM:
                        threshold = 25;
                        break;
                    case DIFFICULTY_HARD:
                        threshold = 35;
                        break;
                }
                
                context->trainingSuccess = (context->minigameState >= threshold);
                completeTraining(context, context->trainingSuccess);
            }
            break;
            
        case TRAINING_TECHNIQUE:
            // Technique training - sequence matching
            // This would normally show a sequence and check player inputs
            // Simplified version: just count correct key presses
            
            // Timer runs down
            context->minigameTimer++;
            
            // Every 30 frames, expect a button press
            if (context->minigameTimer % 30 == 0) {
                // Check if the correct button was pressed
                if (keyPressed(&inputState, KEY_A) || 
                    keyPressed(&inputState, KEY_B) || 
                    keyPressed(&inputState, KEY_L) || 
                    keyPressed(&inputState, KEY_R)) {
                    // Count as success (simplified)
                    context->minigameState++;
                }
            }
            
            // Check if time's up (5 seconds)
            if (context->minigameTimer >= 300) {
                // Calculate success based on correct presses
                int threshold;
                switch (context->selectedDifficulty) {
                    case DIFFICULTY_EASY:
                        threshold = 3;
                        break;
                    case DIFFICULTY_MEDIUM:
                        threshold = 5;
                        break;
                    case DIFFICULTY_HARD:
                        threshold = 8;
                        break;
                }
                
                context->trainingSuccess = (context->minigameState >= threshold);
                completeTraining(context, context->trainingSuccess);
            }
            break;
            
        case TRAINING_SPEED:
            // Speed training - timing based
            // Bar moves back and forth, hit A when in center
            
            // Update bar position (oscillate)
            context->minigameTimer++;
            int barPosition = (context->minigameTimer % 120) - 60;
            if (barPosition < 0) barPosition = -barPosition;
            
            // Check for button press
            if (keyPressed(&inputState, KEY_A)) {
                // Check how close to center (0-30, with 0 being perfect)
                int accuracy = barPosition;
                
                // Different thresholds based on difficulty
                int threshold;
                switch (context->selectedDifficulty) {
                    case DIFFICULTY_EASY:
                        threshold = 20;
                        break;
                    case DIFFICULTY_MEDIUM:
                        threshold = 10;
                        break;
                    case DIFFICULTY_HARD:
                        threshold = 5;
                        break;
                }
                
                context->trainingSuccess = (accuracy <= threshold);
                completeTraining(context, context->trainingSuccess);
            }
            
            // Time limit (10 seconds)
            if (context->minigameTimer >= 600) {
                // Out of time, fail
                completeTraining(context, 0);
            }
            break;
    }
}

// Complete training and apply results
void completeTraining(TrainingContext* context, int success) {
    context->trainingInProgress = 0;
    context->trainingSuccess = success;
    
    if (success) {
        // Calculate and apply training points
        context->trainingPoints = calculateTrainingPoints(context);
        applyTrainingToWeapon(context->player, context->trainingPoints);
        
        // Check if weapon can evolve
        if (canWeaponEvolve(&context->player->weapon)) {
            if (evolveWeapon(&context->player->weapon)) {
                sprintf(context->message, "Success! Your %s evolved!", 
                       context->player->weapon.name);
            } else {
                sprintf(context->message, "Success! Gained %d training points.", 
                       context->trainingPoints);
            }
        } else {
            sprintf(context->message, "Success! Gained %d training points.", 
                   context->trainingPoints);
        }
    } else {
        strcpy(context->message, "Training failed. Try again!");
    }
    
    context->messageTimer = 120;
}

// Calculate training points based on difficulty and success
int calculateTrainingPoints(TrainingContext* context) {
    if (!context->trainingSuccess) {
        return 0;
    }
    
    int basePoints;
    switch (context->selectedDifficulty) {
        case DIFFICULTY_EASY:
            basePoints = 1;
            break;
        case DIFFICULTY_MEDIUM:
            basePoints = 3;
            break;
        case DIFFICULTY_HARD:
            basePoints = 5;
            break;
        default:
            basePoints = 1;
            break;
    }
    
    // Add a small random bonus
    basePoints += getRandomInt(3);
    
    return basePoints;
}

// Apply training points to weapon
void applyTrainingToWeapon(Player* player, int points) {
    // Add points to weapon's training
    player->weapon.trainingPoints += points;
}

// Check if weapon can evolve to next level
int canWeaponEvolve(Weapon* weapon) {
    // Already at maximum evolution
    if (weapon->evolution == EVOLUTION_MASTER) {
        return 0;
    }
    
    // Check if enough training points accumulated
    return (weapon->trainingPoints >= weapon->requiredTrainingForNextLevel);
}

// Render training UI
void renderTrainingUI(TrainingContext* context) {
    // Clear screen
    fillScreen(BLACK);
    
    // Draw title
    printTextAligned(SCREEN_WIDTH / 2, 10, "WEAPON TRAINING", WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
    
    // Draw weapon info
    printText(20, 30, "Weapon:", WHITE);
    printText(80, 30, context->player->weapon.name, YELLOW);
    
    // Draw training progress
    printText(20, 45, "Training Points:", WHITE);
    printNumber(125, 45, context->player->weapon.trainingPoints, GREEN);
    printText(145, 45, "/", WHITE);
    printNumber(155, 45, context->player->weapon.requiredTrainingForNextLevel, WHITE);
    
    // Draw message box
    drawHollowRect(10, 140, 220, 20, WHITE);
    printText(20, 145, context->message, WHITE);
    
    // If in a training session, render the minigame
    if (context->trainingInProgress) {
        renderTrainingMinigame(context);
    } else {
        // Draw training type selection
        printText(20, 70, "Training Type:", WHITE);
        
        Color strengthColor = (context->selectedType == TRAINING_STRENGTH) ? YELLOW : WHITE;
        Color techniqueColor = (context->selectedType == TRAINING_TECHNIQUE) ? YELLOW : WHITE;
        Color speedColor = (context->selectedType == TRAINING_SPEED) ? YELLOW : WHITE;
        
        printText(50, 85, "Strength", strengthColor);
        printText(120, 85, "Technique", techniqueColor);
        printText(190, 85, "Speed", speedColor);
        
        // Draw difficulty selection
        printText(20, 105, "Difficulty:", WHITE);
        
        Color easyColor = (context->selectedDifficulty == DIFFICULTY_EASY) ? YELLOW : WHITE;
        Color mediumColor = (context->selectedDifficulty == DIFFICULTY_MEDIUM) ? YELLOW : WHITE;
        Color hardColor = (context->selectedDifficulty == DIFFICULTY_HARD) ? YELLOW : WHITE;
        
        printText(90, 105, "Easy", easyColor);
        printText(130, 105, "Medium", mediumColor);
        printText(180, 105, "Hard", hardColor);
        
        // Draw instructions
        printText(20, 170, "A: Start Training", BLUE);
        printText(150, 170, "B: Exit", BLUE);
    }
}

// Render training minigame
void renderTrainingMinigame(TrainingContext* context) {
    switch (context->selectedType) {
        case TRAINING_STRENGTH:
            // Strength training - show button presses
            printTextAligned(SCREEN_WIDTH / 2, 70, "STRENGTH TRAINING", 
                        YELLOW, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            
            // Progress bar
            drawHollowRect(40, 90, 160, 20, WHITE);
            
            // Determine target based on difficulty
            int target;
            switch (context->selectedDifficulty) {
                case DIFFICULTY_EASY:
                    target = 15;
                    break;
                case DIFFICULTY_MEDIUM:
                    target = 25;
                    break;
                case DIFFICULTY_HARD:
                    target = 35;
                    break;
            }
            
            // Fill progress bar proportionally
            int fillWidth = (context->minigameState * 160) / target;
            if (fillWidth > 160) fillWidth = 160;
            drawRect(40, 90, fillWidth, 20, GREEN);
            
            // Show count
            printText(40, 120, "Button Presses:", WHITE);
            printNumber(150, 120, context->minigameState, GREEN);
            
            // Show time remaining
            int timeLeft = (180 - context->minigameTimer) / 60;
            if (timeLeft < 0) timeLeft = 0;
            printText(40, 170, "Time Left:", WHITE);
            printNumber(110, 170, timeLeft, YELLOW);
            printText(120, 170, "seconds", YELLOW);
            break;
            
        case TRAINING_TECHNIQUE:
            // Technique training - sequence matching
            printTextAligned(SCREEN_WIDTH / 2, 70, "TECHNIQUE TRAINING", 
                        YELLOW, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            
            // Show sequence (simplified)
            printTextAligned(SCREEN_WIDTH / 2, 90, "Press any button when prompted!", 
                        WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            
            // Show correct presses
            printText(40, 120, "Correct Presses:", WHITE);
            printNumber(150, 120, context->minigameState, GREEN);
            
            // Timer prompt
            if (context->minigameTimer % 30 < 15) {
                printTextAligned(SCREEN_WIDTH / 2, 110, "NOW!", 
                            RED, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            }
            break;
            
        case TRAINING_SPEED:
            // Speed training - timing based
            printTextAligned(SCREEN_WIDTH / 2, 70, "SPEED TRAINING", 
                        YELLOW, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            
            // Bar indicator
            drawHollowRect(40, 90, 160, 20, WHITE);
            
            // Calculate bar position (oscillate)
            int barPosition = (context->minigameTimer % 120) - 60;
            if (barPosition < 0) barPosition = -barPosition;
            
            // Draw moving indicator
            int indicatorPos = 40 + (barPosition * 160) / 60;
            drawRect(indicatorPos - 2, 90, 5, 20, RED);
            
            // Draw target area
            int targetWidth;
            switch (context->selectedDifficulty) {
                case DIFFICULTY_EASY:
                    targetWidth = 40;
                    break;
                case DIFFICULTY_MEDIUM:
                    targetWidth = 20;
                    break;
                case DIFFICULTY_HARD:
                    targetWidth = 10;
                    break;
            }
            
            drawHollowRect(120 - targetWidth/2, 90, targetWidth, 20, GREEN);
            
            // Show instruction
            printTextAligned(SCREEN_WIDTH / 2, 120, "Press A when the bar is in the green zone!", 
                        WHITE, TEXT_ALIGN_CENTER, SCREEN_WIDTH);
            break;
    }
}
