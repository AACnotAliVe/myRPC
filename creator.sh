#!/bin/bash

set -e

PROJECT_ROOT="$(pwd)"
BIN_DIR="$PROJECT_ROOT/bin"
BUILD_DIR="$PROJECT_ROOT/build"
CONF_DIR="/etc/myRPC"
CONFIG_SRC="$PROJECT_ROOT/src/config"
SERVER_BIN="$BIN_DIR/myRPC-server"
CLIENT_BIN="$BIN_DIR/myRPC-client"
USER=$(whoami)

echo "🔧 [1/2] Сборка проекта"
make clean && make

echo "📁 [2/2] Копирование конфигов"
sudo mkdir -p $CONF_DIR
sudo cp "$CONFIG_SRC/myRPC.conf" "$CONF_DIR/"
echo "$USER" | sudo tee "$CONF_DIR/users.conf" > /dev/null

echo "✅ Все тесты завершены"
