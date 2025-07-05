# Makefile for the canvas drawing project

# Compiler and flags
CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm

# Source files
SRCS = main.c canvas.c
OBJS = $(SRCS:.c=.o)

# Output binary name
TARGET = render

# Default rule
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o *.ppm
