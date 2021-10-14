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
#include "./common.h"

#define SOCKET_NAME "/tmp/rtm_sync"
#define MAX_CLIENT_SUPPORTED 32

int system_call_exit_on_failed(int return_value);

void socket_address_init(struct sockaddr_un *address);

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
    clients.len = 0;
    routes.len = 0;

    pthread_t tid;
    int ret = pthread_create(&tid, NULL, (void *(*)(void *)) server, NULL);
    if (ret != 0) {
        printf("Create thread failed, error code: %d\n", ret);
        exit(EXIT_FAILURE);
    }


    char line_buf[BUFSIZ];
    while (fgets(line_buf, BUFSIZ, stdin)) {
        char cmd[128];
        char destination_and_mask[64];
        char gateway[16];
        char oif[32];
        sscanf(line_buf, "%s", cmd);
        if (string_equal_ignore_case(cmd, "CREATE")) {
            scanf(line_buf, "%s", destination_and_mask); // todo: 不知道为啥，到这里就跑飞了
            char *destination = destination_and_mask;
            char *delimiter = strchr(destination_and_mask, '/');
            *delimiter = '\0';
            char mask = *(delimiter + 1);
            scanf(line_buf, "%s", gateway);
            scanf(line_buf, "%s", oif);
            routes_add(&routes, destination, mask, gateway, oif);
            printf("Route %s/%c %s %s created\n", destination, mask, gateway, oif);
        } else if (string_equal_ignore_case(cmd, "UPDATE")) {
            printf("Update...\n");
        } else if (string_equal_ignore_case(cmd, "DELETE")) {
            printf("Deleting...\n");
        } else if (string_equal_ignore_case(cmd, "LIST")) {
            printf("Destination Subnet \t Gate IP \t OIF");
            for (int i = 0; i < routes.len; i++) {
                printf("%s/%d %s %s\n",
                       routes.data[i].destination,
                       routes.data[i].mask,
                       routes.data[i].gateway_ip,
                       routes.data[i].oif);
            }
        } else {
            printf("I have no idea what you are talking about\n");
        }
    }
    is_exit = true;
    pthread_join(tid, NULL);
    return EXIT_SUCCESS;
}

void sync_all_route_table(int data_socket) {

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

int system_call_exit_on_failed(int return_value) {
    if (return_value == -1) {
        perror("");
        exit(EXIT_FAILURE);
    }
    return return_value;
}

void socket_address_init(struct sockaddr_un *address) {
    memset(address, 0, sizeof(struct sockaddr_un));
    address->sun_family = AF_UNIX;
    strncpy(address->sun_path, SOCKET_NAME, sizeof(address->sun_path) - 1);
}