# Compiler
CC = gcc

# Linker flags (for math library)
LDFLAGS = -lm

# Source files
SRC = main.c

# Object files
OBJ = $(SRC:.c=.o)

# Output binary
TARGET = termcaster

# Build rule
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Clean rule
.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
