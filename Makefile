CC=gcc
CFLAGS=-Wall -ggdb -march=native -fno-omit-frame-pointer -fdiagnostics-show-option -std=c11  -I./include/
LIBS=-lm
SOURCES=$(shell find ./src/ -name '*.c' -type f)
OBJECTS=${SOURCES:.c=.o}

all: debug main

main: $(OBJECTS)
	$(CC) $(CFLAGS) -o main $(OBJECTS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

debug:
	@echo "Building with: $(CC) on ${shell uname -s} ${shell uname -m}"

clean:
	rm -f main $(OBJECTS)
