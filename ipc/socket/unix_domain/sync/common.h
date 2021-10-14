#ifndef SOCKET_MSG_H
#define SOCKET_MSG_H

#include <stdbool.h>

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

#endif //SOCKET_MSG_H
