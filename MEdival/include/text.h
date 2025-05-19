#ifndef TEXT_H
#define TEXT_H


#include "gba.h"

// Font constants
#define FONT_WIDTH 8
#define FONT_HEIGHT 8
#define FONT_NUM_CHARS 128

// Text alignment
typedef enum {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
} TextAlignment;

// Initialize text engine
void initTextSystem();

// Print a character
void printChar(int x, int y, char ch, Color color);

// Print text
void printText(int x, int y, const char* text, Color color);

// Print text with alignment
void printTextAligned(int x, int y, const char* text, Color color, TextAlignment alignment, int width);

// Print text in a box with word wrapping
void printTextBox(int x, int y, int width, int height, const char* text, Color textColor, Color bgColor, Color borderColor);

// Print a number
void printNumber(int x, int y, int number, Color color);

// Measure text width in pixels
int measureTextWidth(const char* text);

// Load font data
void loadFont();

#endif // TEXT_H
