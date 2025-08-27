# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 

# Directories
SRC_DIR = src
BIN_DIR = .

# Source and header files
SRC = $(SRC_DIR)/assg.c $(SRC_DIR)/vector.c $(SRC_DIR)/spheres.c $(SRC_DIR)/color.c
HEADERS = $(SRC_DIR)/vector.h $(SRC_DIR)/spheres.h $(SRC_DIR)/color.h

# Targets
all: MS1 MS2 FS

MS1: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -DMS1 -o $(BIN_DIR)/MS1_assg $(SRC) -lm

MS2: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -DMS2 -o $(BIN_DIR)/MS2_assg $(SRC) -lm

FS: $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -DFS -o $(BIN_DIR)/FS_assg $(SRC) -lm

# Clean up generated files
.PHONY: clean
clean:
	rm -f $(BIN_DIR)/MS1_assg $(BIN_DIR)/MS2_assg $(BIN_DIR)/FS_assg