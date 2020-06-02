#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 35
#define BLACK 0
#define LIGHT_GRAY 7

uint16_t* termBuf = (uint16_t*) 0xB8000;

static uint8_t getColor(uint8_t fg, uint8_t bg){
	return fg | bg << 4;
}

size_t curRow = 0;
size_t curCol = 0;
uint8_t curColor;



size_t strlen(const char* string){
	size_t len = 0;
	while(string[len]){
		len++;
	}
	return len;
}

uint16_t getVgaChar(unsigned char c, uint8_t color){
	return (uint16_t) c | (uint16_t) color << 8;
}

void printCharAt(size_t x, size_t y, unsigned char c, uint8_t color){
	const size_t index = SCREEN_WIDTH * y + x;
	termBuf[index] = getVgaChar(c, color);
}

void termInit(void){
	curColor = getColor(LIGHT_GRAY, BLACK);
	for(size_t x = 0; x <= SCREEN_WIDTH; x++){
		for(size_t y = 0; y <= SCREEN_HEIGHT; y++){
			printCharAt(x, y, ' ', curColor);
		}
	}
}

void print(const char* str){
	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++){
		unsigned char c = str[i];
		if(c != '\n')
			printCharAt(curCol, curRow, c, curColor);
		curCol++;
		if (curCol==SCREEN_WIDTH||c=='\n'){
			curCol = 0;
			curRow++;
			if(curRow == SCREEN_HEIGHT){
			curRow = 0;
			//TODO: Make text on screen scroll
			}
		}
	}
}
