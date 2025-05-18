
CC = gcc
CFLAGS = -Wall -I./include
LDFLAGS = -lm

SRCS = $(wildcard source/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = medieval_rpg

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
