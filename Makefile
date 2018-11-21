# Copyright (c) 2010-2014 Christopher Swenson.
# Copyright (c) 2012 Google Inc. All Rights Reserved.

CC ?= gcc
CFLAGS = -O3 -g -Wall -pedantic
TARGET = fast_merge.out

.PHONY: run format clean

default: main.c sort.h
	$(CC) $(CFLAGS) main.c -o $(TARGET)

run: default
	./$(TARGET)

format:
	astyle --options=astyle.options sort.h main.c

clean:
	rm -f $(TARGET)
