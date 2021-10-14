#include "common.h"
#include <memory.h>

bool routes_add(Routes *routes, char *destination, char mask, char *gateway_ip, char *oif) {
    // Todo: safety, check len
    strcpy(routes->data[routes->len].destination, destination);
    routes->data[routes->len].mask = mask;
    strcpy(routes->data[routes->len].gateway_ip, gateway_ip);
    strcpy(routes->data[routes->len].oif, oif);
    routes->len++;
    return true;
}
