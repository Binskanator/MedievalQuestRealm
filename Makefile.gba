# Makefile for Medieval RPG GBA Game

TARGET = game

# DevkitARM paths - adjust these according to your setup
DEVKITARM = /opt/devkitpro/devkitARM
LIBGBA = /opt/devkitpro/libgba

# Tools
CC = $(DEVKITARM)/bin/arm-none-eabi-gcc
OBJCOPY = $(DEVKITARM)/bin/arm-none-eabi-objcopy
GBAFIX = $(DEVKITARM)/bin/gbafix

# Directories
SRC_DIR = source
INCLUDE_DIR = include
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

# Flags and includes
CFLAGS = -mthumb -mthumb-interwork -mcpu=arm7tdmi -mtune=arm7tdmi -O2 -Wall
CFLAGS += -fomit-frame-pointer -ffast-math
CFLAGS += -I$(INCLUDE_DIR) -I$(LIBGBA)/include

LDFLAGS = -mthumb -mthumb-interwork -specs=gba.specs
LDFLAGS += -L$(LIBGBA)/lib -lgba

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Make directories
$(shell mkdir -p $(BUILD_DIR) $(OBJ_DIR))

# Build rules
all: $(BUILD_DIR)/$(TARGET).gba

$(BUILD_DIR)/$(TARGET).gba: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -v -O binary $< $@
	$(GBAFIX) $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean