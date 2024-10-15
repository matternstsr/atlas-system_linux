# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pedantic

# Directories
SRC_DIR = src
BIN_DIR = bin

# Source files
SERVER_SRC = $(SRC_DIR)/tcp_server.c
CLIENT_SRC = $(SRC_DIR)/tcp_client.c

# Output binaries
SERVER_BIN = $(BIN_DIR)/tcp_server
CLIENT_BIN = $(BIN_DIR)/tcp_client

# Targets
all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(CLIENT_BIN): $(CLIENT_SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
