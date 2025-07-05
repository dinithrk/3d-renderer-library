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
all: run

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to run the program and convert the output image
# Run the render program
# Convert the PPM to JPG
run: $(TARGET)
	./$(TARGET)       
	convert clock_lines.ppm -quality 100 clock_lines.jpg 
	rm -f $(TARGET) *.o *.ppm 
	# 
	#Check the created jpg file


	
