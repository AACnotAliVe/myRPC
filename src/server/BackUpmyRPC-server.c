#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <json-c/json.h>

#define BUFFER_SIZE 4096
#define TMP_TEMPLATE_OUT "/tmp/myRPC_XXXXXX.stdout"
#define TMP_TEMPLATE_ERR "/tmp/myRPC_XXXXXX.stderr"

int user_allowed(const char *username) {
    FILE *fp = fopen("/etc/myRPC/users.conf", "r");
    if (!fp) return 0;

    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, username) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

char *read_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return strdup("(empty)");

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *content = malloc(size + 1);
    fread(content, 1, size, fp);
    content[size] = '\0';
    fclose(fp);
    return content;
}

int main() {
    printf("[DEBUG] Запуск myRPC-server\n");
    fflush(stdout);

    int port = 1234;
    int use_stream = 1;

    FILE *conf = fopen("/etc/myRPC/myRPC.conf", "r");
    if (conf) {
        char line[256];
        while (fgets(line, sizeof(line), conf)) {
            if (strstr(line, "port")) sscanf(line, "port = %d", &port);
            if (strstr(line, "socket_type") && strstr(line, "dgram")) use_stream = 0;
        }
        fclose(conf);
    }

    int sockfd = socket(AF_INET, use_stream ? SOCK_STREAM : SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); exit(1); }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY
    };

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); exit(1);
    }

    if (use_stream && listen(sockfd, 5) < 0) {
        perror("listen"); exit(1);
    }

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock < 0) { perror("accept"); continue; }

        char buffer[BUFFER_SIZE] = {0};
        recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        printf("[DEBUG] Получено: %s\n", buffer);

        struct json_object *jobj = json_tokener_parse(buffer);
        const char *login = json_object_get_string(json_object_object_get(jobj, "login"));
        const char *cmd = json_object_get_string(json_object_object_get(jobj, "command"));

        struct json_object *resp = json_object_new_object();

        if (!user_allowed(login)) {
            json_object_object_add(resp, "code", json_object_new_int(1));
            json_object_object_add(resp, "result", json_object_new_string("Access denied"));
        } else {
            char outname_template[] = TMP_TEMPLATE_OUT;
            char errname_template[] = TMP_TEMPLATE_ERR;
            int outfd = mkstemps(outname_template, 7);
            int errfd = mkstemps(errname_template, 7);

            if (outfd < 0 || errfd < 0) {
                json_object_object_add(resp, "code", json_object_new_int(1));
                json_object_object_add(resp, "result", json_object_new_string("Temp file error"));
            } else {
                close(outfd);
                close(errfd);
                char command[1024];
                snprintf(command, sizeof(command), "%s > %s 2> %s", cmd, outname_template, errname_template);
                int ret = system(command);

                char *output = read_file(ret == 0 ? outname_template : errname_template);
                json_object_object_add(resp, "code", json_object_new_int(ret == 0 ? 0 : 1));
                json_object_object_add(resp, "result", json_object_new_string(output));
                free(output);
            }
            unlink(outname_template);
            unlink(errname_template);
        }
            const char *reply = json_object_to_json_string(resp);
            send(client_sock, reply, strlen(reply), 0);

        close(client_sock);
        json_object_put(jobj);
        json_object_put(resp);
        }

        close(sockfd);
    return 0;
}
