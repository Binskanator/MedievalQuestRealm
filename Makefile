# Makefile for Medieval RPG GBA Game

# Set the name of the output ROM
TARGET := game

# GBA tools and compiler settings
CC := gcc
LD := gcc
OBJCOPY := objcopy
CFLAGS := -O2 -Wall 
LDFLAGS := -lm

# Include paths
INCLUDE := -I./include

# Source directories
SOURCES := $(wildcard source/*.c)
OBJECTS := $(SOURCES:.c=.o)
ASM_SOURCES := source/crt0.s
ASM_OBJECTS := $(ASM_SOURCES:.s=.o)

# Build rules
.PHONY: all clean

all: $(TARGET).gba

# For testing on PC (not actual GBA compilation)
$(TARGET).gba: $(OBJECTS) 
        $(LD) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
        touch $(TARGET).gba

# For testing on an emulator/PC
$(TARGET): $(OBJECTS)
        $(LD) -o $@ $^ $(LDFLAGS)

# Compile C files
%.o: %.c
        $(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Assemble ASM files (not used in this simplified version)
%.o: %.s
        $(CC) $(CFLAGS) -c $< -o $@

clean:
        @rm -f $(OBJECTS)
        @rm -f $(ASM_OBJECTS)
        @rm -f $(TARGET)
        @rm -f $(TARGET).gba
        @rm -f $(TARGET).map