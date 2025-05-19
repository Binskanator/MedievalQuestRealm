#include "../include/input.h"

// Input state
InputState inputState;

// Initialize input state
void initInput(InputState* input) {
    input->currentKeys = 0;
    input->previousKeys = 0;
}

// Update input state
void updateInput(InputState* input) {
    input->previousKeys = input->currentKeys;
    input->currentKeys = ~REG_KEYINPUT & KEY_MASK;
}

// Check if a key was just pressed
int keyPressed(InputState* input, unsigned short key) {
    return (input->currentKeys & key) && !(input->previousKeys & key);
}

// Check if a key was just released
int keyReleased(InputState* input, unsigned short key) {
    return !(input->currentKeys & key) && (input->previousKeys & key);
}

// Check if a key is being held
int keyHeld(InputState* input, unsigned short key) {
    return input->currentKeys & key;
}

// Wait for any key press
void waitForKeyPress(InputState* input) {
    while (1) {
        updateInput(input);
        if (input->currentKeys) {
            break;
        }
        waitForVBlank();
    }
    
    // Wait for release
    while (1) {
        updateInput(input);
        if (!input->currentKeys) {
            break;
        }
        waitForVBlank();
    }
}
