CC=gcc
CFLAGS=-ffreestanding -Wall -Wextra -std=gnu11 -O2
BUILD_DIR=../build/

main: 

%.o: %.c
	$(CC) -c $@ -o $(BUILD_DIR)$< $(CFLAGS)

clean:
	rm -f $(BUILD_DIR)/*.o