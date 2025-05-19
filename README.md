# myRPC UNIX

---

Проект, который содержит демон myRPC-server , клиент myRPC-client, библиотеку mysyslog и Bash скрипты 

### Клиент myRPC-client

Пользователь при помощи аргументов командной строки задает параметры подключения к серверу, тип сокета и команду.

### Сервер myRPC-server

Получает из конфигурационного файла /etc/myRPC/myRPC.conf номер порта и тип сокета, получает через механизм сокетов строку в выбранном формате с именем пользователя и командой разбирает строку, получает из нее имя пользователя и команду, проверяет в конфигурационном файле /etc/myRPC/users.conf есть ли этот пользователь.

### Библиотека mysyslog

Логирует действия и команды во временные файлы. 
Успешные действия /tmp/myRPC_XXXXXX.stdout 
Неудачные действия /tmp/myRPC_XXXXXX.stderr

---

## Настройки

Проект имеет настраиваемые конфигурационные файлы: myRPC.conf , myRPC.server.

### myRPC.conf

```
port = <Укажите номер порта>
socket_type = <stream (TCP),dgram (UDP)>
```

Если не настроить то сервер использует по умолчанию порт 25565 и тип сокета stream!

### users.conf

```
<Пользователь 1>
<Пользователь 2>
<Пользователь 3>
```

Если ваш пользователь не будет в этом файле то вам будет отказано в доступе к серверу и он выведет ошибку в формате JSON

### dowload.sh

Загружает все самое необходимое чтобы сервер смог запуститься 

### settings.sh

Собирает данные из конфигурационных файлов и загружает их в /etc/myRPC

### start.sh

После команды make вводим эту команду и запускается сервер. 


## Зависимости 

- gcc (Компилятор языка Си)
- nginx / Apache2 (Сетевой репозиторий)
- git (Клонирование репозитория)
- make (Сборка проекта)

## Сборка проекта

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
