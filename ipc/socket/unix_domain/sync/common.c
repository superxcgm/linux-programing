#include "common.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

bool routes_add(Routes *routes, char *destination, char mask, char *gateway_ip, char *oif) {
    // Todo: safety, check len
    strcpy(routes->data[routes->len].destination, destination);
    routes->data[routes->len].mask = mask;
    strcpy(routes->data[routes->len].gateway_ip, gateway_ip);
    strcpy(routes->data[routes->len].oif, oif);
    routes->len++;
    return true;
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

void print_routes(Routes *routes) {
    printf("%-20s    %-20s %-20s\n","Destination Subnet", "Gate IP", "OIF");
    for (int i = 0; i < routes->len; i++) {
        printf("%s/%-13d %-20s %-20s\n",
               routes->data[i].destination,
               routes->data[i].mask,
               routes->data[i].gateway_ip,
               routes->data[i].oif);
    }
}
