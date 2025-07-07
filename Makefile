# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -lm

# Sources
COMMON_SRCS = src/canvas.c src/math3d.c
LIGHT_SRCS = demo/animation.c src/lightsource_renderer.c src/lightning.c $(COMMON_SRCS)
SOCCER_SRCS = demo/main.c src/renderer.c $(COMMON_SRCS)
CLOCK_SRCS = demo/clock_lines.c src/canvas.c

# Output binaries
BIN = renderer

.PHONY: all light_source soccerball clock_lines clean

all: light_source soccerball clock_lines

light_source:
	gcc $(LIGHT_SRCS) -Iinclude -lm -o $(BIN)
	./$(BIN)
	convert -delay 7 -loop 0 frame_*.ppm -resize 800x800 light_source.gif
	rm -f frame_*.ppm $(BIN)

soccerball:
	gcc $(SOCCER_SRCS) -Iinclude -lm -o $(BIN)
	./$(BIN)
	convert -delay 7 -loop 0 frame_*.ppm -resize 800x800 soccerball.gif
	rm -f frame_*.ppm $(BIN)

clock_lines:
	gcc $(CLOCK_SRCS) -Iinclude -lm -o $(BIN)
	./$(BIN)
	convert clock_lines.ppm -quality 100 clock_lines.jpg
	rm -f clock_lines.ppm $(BIN)

clean:
	rm -f *.o frame_*.ppm renderer
