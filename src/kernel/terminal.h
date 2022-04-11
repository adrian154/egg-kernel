#ifndef __TERMINAL_H
#define __TERMINAL_H

#include "kernel.h"

#define VGA_TEXT_BUFFER_ADDR    0xB8000
#define TERMINAL_WIDTH          80
#define TERMINAL_HEIGHT         25

#define TERINAL_BLINK   0x80
#define BG_BLACK        0x00
#define BG_BLUE         0x10
#define BG_GREEN        0x20
#define BG_CYAN         0x30
#define BG_RED          0x40
#define BG_MAGENTA      0x50
#define BG_BROWN        0x60
#define BG_LIGHT_GRAY   0x70

#define FG_BLACK        0x00
#define FG_DARK_BLUE    0x01
#define FG_DARK_GREEN   0x02
#define FG_DARK_CYAN    0x03
#define FG_DARK_RED     0x04
#define FG_MAGENTA      0x05
#define FG_BROWN        0x06
#define FG_LIGHT_GRAY   0x07
#define FG_DARK_GRAY    0x08
#define FG_BLUE         0x09
#define FG_GREEN        0x0A
#define FG_CYAN         0x0B
#define FG_RED          0x0C
#define FG_PINK         0x0D
#define FG_YELLOW       0x0E
#define FG_WHITE        0x0F

extern void moveCursor(unsigned int x, unsigned int y);
extern void enableCursor();
extern void disableCursor();
extern void clearTerminal();
extern void setchar(unsigned int x, unsigned int y, char character, uint8_t color);
extern void putchar(char character);
extern uint8_t terminalColor;

#endif