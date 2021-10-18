#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/select.h>
#include <pthread.h>
#include <stdbool.h>
#include <ctype.h>
#include <fcntl.h>
#include "./common.h"

#define MAX_CLIENT_SUPPORTED 32

bool string_equal_ignore_case(const char *s1, const char *s2);

void sync_all_route_table(int socket);

bool is_exit = false;

typedef struct clients {
    int len;
    int data[MAX_CLIENT_SUPPORTED];
} Clients;

Clients clients;
Routes routes;

void server() {
    unlink(SOCKET_NAME);

    int master_socket = system_call_exit_on_failed(socket(AF_UNIX, SOCK_STREAM, 0));
    printf("Master socket created\n");

    struct sockaddr_un addr;
    socket_address_init(&addr);

    system_call_exit_on_failed(bind(master_socket, (const struct sockaddr *) &addr, sizeof(struct sockaddr_un)));
    printf("Bind succeed\n");

    system_call_exit_on_failed(listen(master_socket, MAX_CLIENT_SUPPORTED));


    while (!is_exit) {
        int data_socket = accept(master_socket, NULL, NULL);
        clients.data[clients.len++] = data_socket;
        sync_all_route_table(data_socket);
    }

    close(master_socket);
    printf("Server down.\n");
    unlink(SOCKET_NAME);
    exit(EXIT_SUCCESS);
}

int main() {
//    int fd = open("test.in", O_RDONLY);
//    dup2(fd, STDIN_FILENO);
    clients.len = 0;
    routes.len = 0;

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, (void *(*)(void *)) server, NULL);
    if (ret != 0) {
        printf("Create thread failed, error code: %d\n", ret);
        exit(EXIT_FAILURE);
    }


    char *line_buf = malloc(BUFSIZ);
    char *cmd = malloc(64);
    char *destination_and_mask = malloc(64);
    char *gateway = malloc(16);
    char *oif = malloc(32);

    while (fgets(line_buf, BUFSIZ, stdin)) {
        sscanf(line_buf, "%s", cmd);
        if (string_equal_ignore_case(cmd, "CREATE")) {
            sscanf(line_buf + strlen("CREATE"), "%s %s %s", destination_and_mask, gateway, oif);
            char *destination = destination_and_mask;
            char *delimiter = strchr(destination_and_mask, '/');
            *delimiter = '\0';
            char mask = atoi(delimiter + 1);
            routes_add(&routes, destination, mask, gateway, oif);
            printf("Route %s/%d %s %s created\n", destination, mask, gateway, oif);
        } else if (string_equal_ignore_case(cmd, "UPDATE")) {
            printf("Update...\n");
        } else if (string_equal_ignore_case(cmd, "DELETE")) {
            printf("Deleting...\n");
        } else if (string_equal_ignore_case(cmd, "LIST")) {

        } else {
            printf("I have no idea what you are talking about\n");
        }
    }
    is_exit = true;
    pthread_join(tid, NULL);
    return EXIT_SUCCESS;
}

void sync_all_route_table(int data_socket) {
    sync_msg_t msg;
    msg.op_code = CREATE;
    for (int i = 0; i < routes.len; ++i) {
        strcpy(msg.msg_body.destination, routes.data[i].destination);
        msg.msg_body.mask = routes.data[i].mask;
        strcpy(msg.msg_body.gateway_ip, routes.data[i].gateway_ip);
        strcpy(msg.msg_body.oif, routes.data[i].oif);
        system_call_exit_on_failed(write(data_socket, &msg, sizeof(sync_msg_t)));
    }
}


bool string_equal_ignore_case(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        int c1 = tolower(*s1);
        int c2 = tolower(*s2);
        if (c1 != c2) {
            return false;
        }
        s1++;
        s2++;
    }
    return *s1 == *s2 && *s1 == '\0';
}
