#ifndef INPUT_H
#define INPUT_H

#include "gba.h"

// Input state structure
typedef struct {
    unsigned short current;
    unsigned short previous;
} InputState;

// Initialize input state
void initInput(InputState* input);

// Update input state
void updateInput(InputState* input);

// Check if a key was just pressed
int keyPressed(InputState* input, unsigned short key);

// Check if a key was just released
int keyReleased(InputState* input, unsigned short key);

// Check if a key is being held
int keyHeld(InputState* input, unsigned short key);

// Wait for any key press
void waitForKeyPress(InputState* input);

extern InputState inputState;

#endif // INPUT_H
