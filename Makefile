CC ?= gcc
CFLAGS = -O3 -g -Wall -pedantic
TARGET = fast_merge.out

.PHONY: run format clean

default: main.c sort.h
	$(CC) $(CFLAGS) main.c -o $(TARGET)

asm: main.c sort.h merge_asm_int64.h
	$(CC) $(CFLAGS) -D ASM main.c -o $(TARGET)

static: main.c sort.h
	$(CC) $(CFLAGS) -static main.c -o ./bin/$(TARGET)

run: default
	./$(TARGET)

format:
	astyle --options=astyle.options sort.h main.c

clean:
	rm -f $(TARGET)
