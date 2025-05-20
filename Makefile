CC = gcc
CFLAGS = -Wall -Isrc/mysyslog
LDFLAGS =
JSONC_FLAGS = $(shell pkg-config --cflags --libs json-c)

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

CLIENT_SRC = $(SRC_DIR)/client/myRPC-client.c
SERVER_SRC = $(SRC_DIR)/server/myRPC-server.c
LIB_SRC    = $(SRC_DIR)/mysyslog/mysyslog.c

CLIENT_OBJ = $(BUILD_DIR)/myRPC-client.o
SERVER_OBJ = $(BUILD_DIR)/myRPC-server.o
LIB_OBJ    = $(BUILD_DIR)/mysyslog.o

CLIENT_BIN = $(BIN_DIR)/myRPC-client
SERVER_BIN = $(BIN_DIR)/myRPC-server

all: dirs $(CLIENT_BIN) $(SERVER_BIN)

dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

$(CLIENT_BIN): $(CLIENT_OBJ) $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(JSONC_FLAGS)

$(SERVER_BIN): $(SERVER_OBJ) $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(JSONC_FLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/client/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/server/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/mysyslog.o: $(SRC_DIR)/mysyslog/mysyslog.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	rm -rf build-deb
	bash clean.sh

.PHONY: all clean dirs
