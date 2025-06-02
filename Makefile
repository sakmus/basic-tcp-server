# Compiler and flags
CC = gcc
CFLAGS = -O2 -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Common source files
COMMON_SRCS = $(SRC_DIR)/functions.c

# Server-specific source files
SERVER_SRCS = $(SRC_DIR)/server.c $(SRC_DIR)/server_functions.c $(COMMON_SRCS)

# Client-specific source files
CLIENT_SRCS = $(SRC_DIR)/client.c $(SRC_DIR)/client_functions.c $(COMMON_SRCS)

# Object files
SERVER_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SERVER_SRCS))
CLIENT_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(CLIENT_SRCS))

# Target binaries
SERVER_TARGET = $(BIN_DIR)/server
CLIENT_TARGET = $(BIN_DIR)/client

.PHONY: all server client clean

# Build both server and client
all: $(SERVER_TARGET) $(CLIENT_TARGET)

# Build only server
server: $(SERVER_TARGET)

# Build only client
client: $(CLIENT_TARGET)

# Create bin and build directories if they don't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Link server object files to create server executable
$(SERVER_TARGET): $(BIN_DIR) $(BUILD_DIR) $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o $@

# Link client object files to create client executable
$(CLIENT_TARGET): $(BIN_DIR) $(BUILD_DIR) $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o $@

# Compile .c files to .o files inside build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build and bin directories
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
