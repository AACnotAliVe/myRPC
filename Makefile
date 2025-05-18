CC = gcc
CFLAGS = -Wall -Isrc/mysyslog
LDFLAGS =
JSONC_FLAGS = $(shell pkg-config --cflags --libs json-c)

# Каталоги
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Исходники
CLIENT_SRC = $(SRC_DIR)/client/myRPC-client.c
SERVER_SRC = $(SRC_DIR)/server/myRPC-server.c
LIB_SRC    = $(SRC_DIR)/mysyslog/mysyslog.c

# Объекты
CLIENT_OBJ = $(BUILD_DIR)/myRPC-client.o
SERVER_OBJ = $(BUILD_DIR)/myRPC-server.o
LIB_OBJ    = $(BUILD_DIR)/mysyslog.o

# Бинарники
CLIENT_BIN = $(BIN_DIR)/myRPC-client
SERVER_BIN = $(BIN_DIR)/myRPC-server

# Цель по умолчанию
all: dirs $(CLIENT_BIN) $(SERVER_BIN)

# Создание директорий
dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Сборка клиентского бинарника
$(CLIENT_BIN): $(CLIENT_OBJ) $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(JSONC_FLAGS)

# Сборка серверного бинарника
$(SERVER_BIN): $(SERVER_OBJ) $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(JSONC_FLAGS)

# Компиляция .o из исходников
$(BUILD_DIR)/%.o: $(SRC_DIR)/client/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/server/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# 🔽 ДОБАВЬ ЭТО правило для mysyslog
$(BUILD_DIR)/mysyslog.o: $(SRC_DIR)/mysyslog/mysyslog.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

# Очистка сборки
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean dirs
