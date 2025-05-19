#ifndef TRAINING_H
#define TRAINING_H

#include "characters.h"

// Training types
typedef enum {
    TRAINING_STRENGTH,
    TRAINING_TECHNIQUE,
    TRAINING_SPEED
} TrainingType;

// Training difficulty
typedef enum {
    DIFFICULTY_EASY,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD
} TrainingDifficulty;

// Training context for the UI
typedef struct {
    Player* player;
    TrainingType selectedType;
    TrainingDifficulty selectedDifficulty;
    int trainingInProgress;
    int trainingSuccess;
    int trainingPoints;
    int minigameTimer;
    int minigameState;
    char message[50];
    int messageTimer;
} TrainingContext;

// Initialize training context
void initTraining(TrainingContext* context, Player* player);

// Start training minigame
void startTraining(TrainingContext* context);

// Update training minigame state
void updateTraining(TrainingContext* context);

// Complete training and apply results
void completeTraining(TrainingContext* context, int success);

// Calculate training points based on difficulty and success
int calculateTrainingPoints(TrainingContext* context);

// Apply training points to weapon
void applyTrainingToWeapon(Player* player, int points);

// Check if weapon can evolve to next level
int canWeaponEvolve(Weapon* weapon);

// Render training UI
void renderTrainingUI(TrainingContext* context);

// Render training minigame
void renderTrainingMinigame(TrainingContext* context);

#endif // TRAINING_H
