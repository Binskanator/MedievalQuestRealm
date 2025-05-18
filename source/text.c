#include "../include/text.h"
#include <string.h>

// Font data - a simple 8x8 pixel font
// Each character is represented as 8 bytes (8x8 pixels)
// This is a minimal font with just basic characters
unsigned short fontData[128 * 8] = {0};

// Initialize text engine
void initTextSystem() {
    // Initialize simple font data
    // Space (ASCII 32)
    memset(&fontData[32 * 8], 0, 8 * sizeof(unsigned short));
    
    // Exclamation point (ASCII 33)
    fontData[33 * 8 + 0] = 0x0100;
    fontData[33 * 8 + 1] = 0x0100;
    fontData[33 * 8 + 2] = 0x0100;
    fontData[33 * 8 + 3] = 0x0100;
    fontData[33 * 8 + 4] = 0x0100;
    fontData[33 * 8 + 5] = 0x0000;
    fontData[33 * 8 + 6] = 0x0100;
    fontData[33 * 8 + 7] = 0x0000;
    
    // Period (ASCII 46)
    fontData[46 * 8 + 0] = 0x0000;
    fontData[46 * 8 + 1] = 0x0000;
    fontData[46 * 8 + 2] = 0x0000;
    fontData[46 * 8 + 3] = 0x0000;
    fontData[46 * 8 + 4] = 0x0000;
    fontData[46 * 8 + 5] = 0x0000;
    fontData[46 * 8 + 6] = 0x0100;
    fontData[46 * 8 + 7] = 0x0000;
    
    // '0' to '9' (ASCII 48-57)
    // A simple representation of digits
    for (int i = 0; i < 10; i++) {
        fontData[(48 + i) * 8 + 0] = 0x0380;
        fontData[(48 + i) * 8 + 1] = 0x0440;
        fontData[(48 + i) * 8 + 2] = 0x0440;
        fontData[(48 + i) * 8 + 3] = 0x0440;
        fontData[(48 + i) * 8 + 4] = 0x0440;
        fontData[(48 + i) * 8 + 5] = 0x0440;
        fontData[(48 + i) * 8 + 6] = 0x0380;
        fontData[(48 + i) * 8 + 7] = 0x0000;
    }
    
    // 'A' to 'Z' (ASCII 65-90)
    // Simple uppercase letters
    
    // 'A'
    fontData[65 * 8 + 0] = 0x0380;
    fontData[65 * 8 + 1] = 0x0440;
    fontData[65 * 8 + 2] = 0x0440;
    fontData[65 * 8 + 3] = 0x07C0;
    fontData[65 * 8 + 4] = 0x0440;
    fontData[65 * 8 + 5] = 0x0440;
    fontData[65 * 8 + 6] = 0x0440;
    fontData[65 * 8 + 7] = 0x0000;
    
    // 'B'
    fontData[66 * 8 + 0] = 0x0780;
    fontData[66 * 8 + 1] = 0x0440;
    fontData[66 * 8 + 2] = 0x0440;
    fontData[66 * 8 + 3] = 0x0780;
    fontData[66 * 8 + 4] = 0x0440;
    fontData[66 * 8 + 5] = 0x0440;
    fontData[66 * 8 + 6] = 0x0780;
    fontData[66 * 8 + 7] = 0x0000;
    
    // Simple patterns for the rest of uppercase letters
    for (int i = 2; i < 26; i++) {
        fontData[(65 + i) * 8 + 0] = 0x0380;
        fontData[(65 + i) * 8 + 1] = 0x0440;
        fontData[(65 + i) * 8 + 2] = 0x0040;
        fontData[(65 + i) * 8 + 3] = 0x0040;
        fontData[(65 + i) * 8 + 4] = 0x0040;
        fontData[(65 + i) * 8 + 5] = 0x0440;
        fontData[(65 + i) * 8 + 6] = 0x0380;
        fontData[(65 + i) * 8 + 7] = 0x0000;
    }
    
    // 'a' to 'z' (ASCII 97-122)
    // Simple lowercase letters (same as uppercase for simplicity)
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 8; j++) {
            fontData[(97 + i) * 8 + j] = fontData[(65 + i) * 8 + j];
        }
    }
    
    // Load the font data
    loadFont();
}

// Print a character
void printChar(int x, int y, char ch, Color color) {
    if (ch < 0 || ch >= 128) {
        ch = '?'; // Default for non-ASCII characters
    }
    
    // Get the character's bitmap
    unsigned short* charBitmap = &fontData[ch * 8];
    
    // Draw each pixel of the character
    for (int cy = 0; cy < 8; cy++) {
        unsigned short row = charBitmap[cy];
        for (int cx = 0; cx < 8; cx++) {
            if (row & (1 << (15 - cx))) {
                setPixel(x + cx, y + cy, color);
            }
        }
    }
}

// Print text
void printText(int x, int y, const char* text, Color color) {
    int currentX = x;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == '\n') {
            // Newline
            currentX = x;
            y += 10;
        } else {
            printChar(currentX, y, text[i], color);
            currentX += 8; // Character width
        }
    }
}

// Print text with alignment
void printTextAligned(int x, int y, const char* text, Color color, TextAlignment alignment, int width) {
    int textWidth = measureTextWidth(text);
    int startX = x;
    
    switch (alignment) {
        case TEXT_ALIGN_LEFT:
            // Keep startX as is
            break;
        case TEXT_ALIGN_CENTER:
            startX = x - (textWidth / 2);
            break;
        case TEXT_ALIGN_RIGHT:
            startX = x - textWidth;
            break;
    }
    
    printText(startX, y, text, color);
}

// Print text in a box with word wrapping
void printTextBox(int x, int y, int width, int height, const char* text, Color textColor, Color bgColor, Color borderColor) {
    // Draw the box
    drawRect(x, y, width, height, bgColor);
    drawHollowRect(x, y, width, height, borderColor);
    
    // Prepare for word wrapping
    int currentX = x + 4;
    int currentY = y + 4;
    int wordWidth = 0;
    int i = 0;
    int lineStart = 0;
    
    while (text[i] != '\0') {
        // Measure the next word
        wordWidth = 0;
        int j = i;
        
        while (text[j] != ' ' && text[j] != '\0') {
            wordWidth += 8; // Each character is 8 pixels wide
            j++;
        }
        
        // Check if this word would exceed the box width
        if (currentX + wordWidth > x + width - 4) {
            // Move to the next line
            currentX = x + 4;
            currentY += 10;
            
            // Check if we've exceeded the box height
            if (currentY > y + height - 12) {
                break;
            }
        }
        
        // Print the word
        for (; i < j; i++) {
            printChar(currentX, currentY, text[i], textColor);
            currentX += 8;
        }
        
        // Print the space or move to next line
        if (text[i] == ' ') {
            if (currentX + 8 <= x + width - 4) {
                printChar(currentX, currentY, ' ', textColor);
                currentX += 8;
            } else {
                currentX = x + 4;
                currentY += 10;
                
                // Check if we've exceeded the box height
                if (currentY > y + height - 12) {
                    break;
                }
            }
            i++;
        }
    }
}

// Print a number
void printNumber(int x, int y, int number, Color color) {
    char buffer[12]; // Large enough for 32-bit integers
    int i = 0;
    int isNegative = 0;
    
    // Handle negative numbers
    if (number < 0) {
        isNegative = 1;
        number = -number;
    }
    
    // Handle the case when number is 0
    if (number == 0) {
        buffer[i++] = '0';
    }
    
    // Convert number to string in reverse
    while (number > 0) {
        buffer[i++] = '0' + (number % 10);
        number /= 10;
    }
    
    // Add negative sign if needed
    if (isNegative) {
        buffer[i++] = '-';
    }
    
    // Print the number in correct order
    int currentX = x;
    for (int j = i - 1; j >= 0; j--) {
        printChar(currentX, y, buffer[j], color);
        currentX += 8;
    }
}

// Measure text width in pixels
int measureTextWidth(const char* text) {
    return strlen(text) * 8; // Each character is 8 pixels wide
}

// Load font data
void loadFont() {
    // In a real game, this would load font data from ROM
    // Here we've already initialized it in initTextSystem
}
