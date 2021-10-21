#include "./common.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

Routes routes;

int main() {
    routes.len = 0;
    int data_socket = system_call_exit_on_failed(socket(AF_UNIX, SOCK_STREAM, 0));
    struct sockaddr_un address;
    socket_address_init(&address);
    system_call_exit_on_failed(connect(data_socket, (const struct sockaddr *)&address, sizeof (struct sockaddr_un)));
    sync_msg_t msg;

    printf("Connected to server\n");

    while (true) {
        system_call_exit_on_failed(read(data_socket, &msg, sizeof(sync_msg_t)));
        if (msg.op_code == CREATE) {
            printf("Get CREATE msg from server\n");
            routes_add(&routes, msg.msg_body.destination, msg.msg_body.mask, msg.msg_body.gateway_ip, msg.msg_body.oif);
        } else if (msg.op_code == DELETE) {
            printf("Get DELETE msg from server\n");
            routes_delete(&routes, msg.msg_body.destination, msg.msg_body.mask);
        }

        print_routes(&routes);
    }
}