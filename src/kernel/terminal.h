#ifndef __TERMINAL_H
#define __TERMINAL_H

#include "kernel.h"

#define TERMINAL_VGA_TEXT_BUFFER_ADDR 0xB8000
#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 25

#define TERMINAL_COLOR_BLACK 0
#define TERMINAL_COLOR_DARK_BLUE 1
#define TERMINAL_COLOR_DARK_GREEN 2
#define TERMINAL_COLOR_DARK_CYAN 3
#define TERMINAL_COLOR_DARK_RED 4
#define TERMINAL_COLOR_DARK_MAGENTA 5
#define TERMINAL_COLOR_DARK_BROWN 6
#define TERMINAL_COLOR_LIGHT_GRAY 7
#define TERMINAL_COLOR_DARK_GRAY 8
#define TERMINAL_COLOR_BLUE 9
#define TERMINAL_COLOR_GREEN 10
#define TERMINAL_COLOR_CYAN 11
#define TERMINAL_COLOR_RED 12
#define TERMINAL_COLOR_MAGENTA 13
#define TERMINAL_COLOR_YELLOW 14
#define TERINAL_COLOR_WHITE 15

inline uint8_t makeColor(uint8_t foreground, uint8_t background) {
    return background << 4 | foreground;
}

inline uint16_t makeTextElement(char character, uint8_t color) {
    return ((uint16_t)color) << 8 | character;
}

extern void moveCursor(unsigned int x, unsigned int y);
extern void enableCursor();
extern void disableCursor();
extern void initTerminal();
extern void resetTerminal();
extern void clearTerminal();
extern void setChar(unsigned int x, unsigned int y, char character, uint8_t color);
extern void putChar(char character);
extern void print(const char *string);

#endif