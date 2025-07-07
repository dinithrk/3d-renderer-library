# Makefile for compiling, running, and converting frames

# Variables
CC = gcc
CFLAGS = -Iinclude -lm
SRC = demo/main.c src/renderer.c src/canvas.c src/math3d.c
OBJ = renderer
GIF_OUTPUT = soccer_ball.gif
PPM_FILES = frame_*.ppm

# Targets
all: $(OBJ) run convert clean

# Compile the code
$(OBJ): $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $(OBJ)

# Run the renderer
run: $(OBJ)
	./$(OBJ)

# Convert ppm files to gif
convert: $(OBJ)
	convert -delay 7 -loop 0 $(PPM_FILES) -resize 800x800 $(GIF_OUTPUT)

# Clean up generated ppm files
clean:
	rm -f $(PPM_FILES)
	rm -f $(OBJ)

.PHONY: all run convert clean
