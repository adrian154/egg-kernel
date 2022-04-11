#include "terminal.h"
#include "ioport.h"

// Terminal driver using VGA text mode

// terminal state
unsigned int terminalX;
unsigned int terminalY;
uint8_t terminalColor = FG_WHITE | BG_BLACK;
uint16_t *terminalBuffer = (uint16_t *)VGA_TEXT_BUFFER_ADDR;

// TODO: Get rid of magic numbers in cursor functions
// They're VGA registers, but which???
void moveCursor(unsigned int x, unsigned int y) {
    uint16_t position = y * TERMINAL_WIDTH + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)(position >> 8) & 0xFF);
}
 
void enableCursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, inb(0x3D5) & 0xC0);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | 15);
}

void disableCursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void clearTerminal() {

    // reset color
    for(int x = 0; x < TERMINAL_WIDTH; x++) {
        for(int y = 0; y < TERMINAL_HEIGHT; y++) {
            setchar(x, y, ' ', terminalColor);
        }
    }

    // move to top
    terminalX = 0;
    terminalY = 0;

}

void setchar(unsigned int x, unsigned int y, char character, uint8_t color) {
    terminalBuffer[y * TERMINAL_WIDTH + x] = (uint16_t)color << 8 | character;
}

void putchar(char character) {

    if(character == '\r') {
        terminalX = 0;  // carriage return: move cursor to start of line
    } else if(character == '\b') {
        if(terminalX > 0) {
            terminalX--;
        }
    } else if(character == '\n') {
        terminalX = 0;  // do a carriage return anyways, to support both CRLF and LF
        terminalY++;    // advance to next line
    } else {
        setchar(terminalX, terminalY, character, terminalColor);
        terminalX++;
    }

    // if the edge of the terminal has been reached, move to the next line
    if(terminalX >= TERMINAL_WIDTH) {
        terminalX = 0;
        terminalY++;
    }

    // if the bottom of the screen has been reached, scroll upwards
    if(terminalY >= TERMINAL_HEIGHT) {

        // copy contents up
        for(int y = 1; y < TERMINAL_HEIGHT; y++) {
            for(int x = 0; x < TERMINAL_WIDTH; x++) {
                terminalBuffer[(y - 1) * TERMINAL_WIDTH + x] = terminalBuffer[y * TERMINAL_WIDTH + x];
            }
        }

        // reset bottommost line
        for(int x = 0; x < TERMINAL_WIDTH; x++) {
            setchar(x, TERMINAL_HEIGHT - 1, ' ', terminalColor);
        }

        terminalY = TERMINAL_HEIGHT - 1;

    }   
    
    // update cursor
    moveCursor(terminalX, terminalY);

}