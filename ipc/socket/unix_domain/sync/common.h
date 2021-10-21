#ifndef SOCKET_MSG_H
#define SOCKET_MSG_H

#include <stdbool.h>
#include <sys/un.h>

#define SOCKET_NAME "/tmp/rtm_sync"

typedef enum {
    CREATE,
    UPDATE,
    DELETE
} OP_CODE;

typedef struct msg_body {
    char destination[16];
    char mask;
    char gateway_ip[16];
    char oif[32];
} msg_body_t;

typedef struct sync_msg {
    OP_CODE op_code;
    msg_body_t msg_body;
} sync_msg_t;

typedef struct route {
    char destination[16];
    char mask;
    char gateway_ip[16];
    char oif[32];
} Route;

typedef struct routes {
    int len;
    Route data[256];
} Routes;

bool routes_add(Routes *routes, char *destination, char mask, char *gateway_ip, char *oif);
bool routes_delete(Routes *routes, char *destination, char mask);

int system_call_exit_on_failed(int return_value);
void socket_address_init(struct sockaddr_un *address);
void print_routes(Routes *routes);
#endif //SOCKET_MSG_H
