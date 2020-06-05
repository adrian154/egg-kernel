#include "terminal.h"
#include "ioport.h"

// note:
// TECHNICALLY text mode is deprecated,
// but so is BIOS so that's not really relevant

// Some terminal globals
unsigned int terminalX;
unsigned int terminalY;
uint8_t terminalColor;
uint16_t *terminalBuffer;

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

void initTerminal() {
    terminalBuffer = (uint16_t *)TERMINAL_VGA_TEXT_BUFFER_ADDR;
    resetTerminal();
}

void resetTerminal() {
    terminalColor = 0x0F;
    clearTerminal();
    terminalX = 0;
    terminalY = 0;
}

void clearTerminal() {
    for(unsigned int x = 0; x < TERMINAL_WIDTH; x++) {
        for(unsigned int y = 0; y < TERMINAL_HEIGHT; y++) {
            setChar(x, y, ' ', terminalColor);
        }
    }
}

void setChar(unsigned int x, unsigned int y, char character, uint8_t color) {
    terminalBuffer[y * TERMINAL_WIDTH + x] = makeTextElement(character, color);
}

void putChar(char character) {

    // If the cursor is offscreen, wrap.
    if(terminalX == 80) {
        terminalX = 0;
        terminalY++;
    } else if(character == '\r') {
        // Carriage return doesn't advance to a new line.
        // This helps display CRLF or LF accurately
        terminalX = 0;
    } else if(character == '\n') {
        terminalX = 0;
        terminalY++;
    } else {
        setChar(terminalX, terminalY, character, terminalColor);
        terminalX++;
    }

    // Do scroll
    if(terminalY == 25) {
        for(unsigned int y = 1; y < TERMINAL_HEIGHT; y++) {
            for(unsigned int x = 0; x < TERMINAL_WIDTH; x++) {
                terminalBuffer[(y - 1) * TERMINAL_WIDTH + x] = terminalBuffer[y * TERMINAL_WIDTH + x];
            }
        }
        for(unsigned int x = 0; x < TERMINAL_WIDTH; x++) {
            setChar(x, TERMINAL_HEIGHT - 1, ' ', terminalColor);
        }
        terminalY = TERMINAL_HEIGHT - 1;
    }

    // Update displayed cursor
    moveCursor(terminalX, terminalY);

}

void print(const char *string) {
    for(const char *str = string; *str != '\0'; str++) {
        putChar(*str);
    }    
}
