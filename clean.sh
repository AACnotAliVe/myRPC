#!/bin/bash

set -e

echo "Начинаю очищать лог файлы"

find /tmp/ -maxdepth 1 -type f -name 'myRPC*' -exec rm -f {} +

echo "Очистка завершена"
