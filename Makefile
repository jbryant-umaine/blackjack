# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Source files
SRC = main.c stringManip.c stack.c

# Object files
OBJ = $(SRC:.c=.o)

# Executable name
EXEC = main

# Default target
all: $(EXEC)

# Linking the object files to create the executable
$(EXEC): $(OBJ)
	$(CC) -o $@ $^

# Compiling each source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean command to remove object files and the executable
clean:
	rm -f $(OBJ) $(EXEC)