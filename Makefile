CC ?= gcc
CFLAGS = -O3 -g -Wall -pedantic
SRC = src/main.c
TARGET = fast_merge.out

.PHONY: run asm format clean

default: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

static: $(SRC)
	$(CC) $(CFLAGS) -static $(SRC) -o ./bin/$(TARGET)

run: default
	./$(TARGET)

asm: $(SRC)
	$(CC) $(CFLAGS) -D ASM $(SRC) -o $(TARGET)
	./$(TARGET)

format:
	astyle --options=astyle.options ./src/*

clean:
	rm -f $(TARGET)
