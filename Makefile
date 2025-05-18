# Makefile for GBA Development

# Set the name of the output ROM
TARGET := game

# GBA tools and compiler settings
CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy
CFLAGS := -mthumb -mthumb-interwork -O2 -Wall -fno-strict-aliasing
LDFLAGS := -mthumb -mthumb-interwork -Wl,-Map,$(TARGET).map

# Add GBA-specific flags
CFLAGS += -mcpu=arm7tdmi -mtune=arm7tdmi
CFLAGS += -fomit-frame-pointer -ffast-math

# Include paths
INCLUDE := -I./include

# libgba path and libraries
LIBGBA := -lmm -lgba

# Source directories
SOURCES := $(wildcard source/*.c)
OBJECTS := $(SOURCES:.c=.o)

# Build rules
.PHONY: all clean

all: $(TARGET).gba

$(TARGET).gba: $(TARGET).elf
	$(OBJCOPY) -v -O binary $< $@
	gbafix $@

$(TARGET).elf: $(OBJECTS)
	$(CC) $^ $(LDFLAGS) $(LIBGBA) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -f $(OBJECTS)
	@rm -f $(TARGET).elf
	@rm -f $(TARGET).gba
	@rm -f $(TARGET).map