#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <json-c/json.h>
#include <getopt.h>
#include <pwd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 4096

char *escape_special_chars(const char *src) {
    size_t len = strlen(src);
    char *escaped = malloc(len * 2 + 1); // worst case: every char is escaped
    if (!escaped) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    char *dst = escaped;
    while (*src) {
        if (*src == '\"' || *src == '\\') {
            *dst++ = '\\';
        }
        *dst++ = *src++;
    }
    *dst = '\0';
    return escaped;
}

void print_usage(const char *prog) {
    printf("Usage: %s [OPTIONS]\n", prog);
    printf("Options:\n");
    printf("  -c, --command \"bash_command\"    Bash command to execute\n");
    printf("  -h, --host \"ip_addr\"             Server IP address\n");
    printf("  -p, --port PORT                  Server port\n");
    printf("  -s, --stream                     Use stream (TCP) socket\n");
    printf("  -d, --dgram                      Use datagram (UDP) socket\n");
    printf("      --help                       Show this help message\n");
}

int main(int argc, char *argv[]) {
    char *command = NULL, *host = NULL;
    int port = 0, use_stream = 0, use_dgram = 0;

    static struct option long_options[] = {
        {"command", required_argument, 0, 'c'},
        {"host", required_argument, 0, 'h'},
        {"port", required_argument, 0, 'p'},
        {"stream", no_argument, 0, 's'},
        {"dgram", no_argument, 0, 'd'},
        {"help", no_argument, 0, 0},
        {0, 0, 0, 0}
    };

    int opt, option_index = 0;
    while ((opt = getopt_long(argc, argv, "c:h:p:sd", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c': command = strdup(optarg); break;
            case 'h': host = strdup(optarg); break;
            case 'p': port = atoi(optarg); break;
            case 's': use_stream = 1; break;
            case 'd': use_dgram = 1; break;
            case 0:
                if (strcmp(long_options[option_index].name, "help") == 0) {
                    print_usage(argv[0]);
                    exit(0);
                }
                break;
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (!command || !host || !port || (!use_stream && !use_dgram)) {
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    struct passwd *pw = getpwuid(getuid());
    if (!pw) {
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }
    const char *username = pw->pw_name;

    // Экранируем команду
    char *escaped_cmd = escape_special_chars(command);

    // Формируем JSON
    struct json_object *req = json_object_new_object();
    json_object_object_add(req, "login", json_object_new_string(username));
    json_object_object_add(req, "command", json_object_new_string(escaped_cmd));
    const char *json_str = json_object_to_json_string(req);

    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, (use_stream ? SOCK_STREAM : SOCK_DGRAM), 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, host, &servaddr.sin_addr);

    if (use_stream) {
        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
            perror("connect");
            exit(EXIT_FAILURE);
        }
        send(sockfd, json_str, strlen(json_str), 0);
        char buf[BUF_SIZE] = {0};
        recv(sockfd, buf, sizeof(buf), 0);
        printf("Server Response: %s\n", buf);
    } else {
        sendto(sockfd, json_str, strlen(json_str), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
        char buf[BUF_SIZE] = {0};
        socklen_t len = sizeof(servaddr);
        recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&servaddr, &len);
        printf("Server Response: %s\n", buf);
    }

    close(sockfd);
    free(command);
    free(host);
    free(escaped_cmd);
    json_object_put(req);

    return 0;
}

