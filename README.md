# myRPC UNIX

---

Проект, который содержит демон myRPC-server , клиент myRPC-client, библиотеку libmysyslog

Клиент myRPC-client. Пользователь при помощи аргументов командной строки в утилите myRCP-client задает параметры подключения к серверу, тип сокета и команду.

Сервер myRPC-server. Получает из конфигурационного файла /etc/myRPC/ myRPC.conf номер порта и тип сокета, получает через механизм сокетов строку в выбранном формате
 с именем пользователя и командой разбирает строку, получает из нее имя пользователя и команду, проверяет в конфигурационном файле /etc/myRPC/users.conf есть ли этот пользователь.

---

## Настройки

Проект имеет настраиваемые конфигурационные файлы: myRPC.conf , myRPC.server.

# myRPC.conf

port = <Укажите номер порта>
socket_type = <stream (TCP),dgram (UDP)>
```
Если не настроить то сервер берет порт по умолчанию 25565 и тип сокета stream!
```



## Dependencies

- gcc
- nginx
- git

## Building

For building the application, you need:

1. JDK 8 or later
2. Android SDK
3. Android NDK
4. CMake 3.22.1 or later

To build the application:

1. Clone the repository with submodules:
   ```bash
   git clone --recurse-submodules
   ```
2. Run the build script from the root of the repository:
   ```bash
   ./gradlew assembleRelease
   ```
3. The APK will be in `app/build/outputs/apk/release/`

## Запуск
