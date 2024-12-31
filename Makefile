CC=avr-gcc
OBJCOPY=avr-objcopy
MCU=atmega8a
CFLAGS=-mmcu=$(MCU) -Os -Wall -ggdb -fno-omit-frame-pointer -fdiagnostics-show-option -std=c11 -I./include/ -I/usr/lib/avr/include
LIBS=
SOURCES=$(shell find ./src/ -name '*.c' -type f)
OBJECTS=$(SOURCES:.c=.o)
ELF=main.elf
HEX=main.hex
DEBUG=$(shell find . -name '*.vcd' -type f)

all: debug $(HEX)

# This creates the hex file from the elf file
# but ensures that the elf file is created first.
$(HEX): $(ELF)
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

# This creates the elf file from the object files
# but ensures that the object files are created first.
$(ELF): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# This creates the object files from the source files.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Debugging Information.
debug:
	@echo "Building with: $(CC) on $(shell uname -s) $(shell uname -m)"
	@echo "Compiling for Controller: $(MCU)"

clean:
	rm $(OBJECTS) $(ELF) $(HEX) $(DEBUG)
