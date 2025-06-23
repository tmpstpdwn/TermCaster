# Compiler and flags
CC = gcc

# List of source files
SRCS = main.c camera.c map.c raycaster.c renderer.c utils.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output binary name
TARGET = termcaster

# Default rule
all: $(TARGET)

# Link the final binary
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) -lm

# Compile source files into object files
%.o: %.c
	$(CC) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
