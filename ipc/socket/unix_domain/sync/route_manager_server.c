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

#define SOCKET_NAME "/tmp/rtm_sync"
#define MAX_CLIENT_SUPPORTED 32

void monitored_fd_set_add(int *monitored_fd_set, size_t size, int socket_fd);
void monitored_fd_set_init(int *monitored_fd_set, size_t size);
int system_call_exit_on_failed(int return_value);
void socket_addr_init(struct sockaddr_un *addr);
bool string_equal_ignore_case(const char *s1, const char *s2);

void refresh_fd_set(const int *monitored_fd_set, size_t size, fd_set *fd_set_ptr);
int get_max_fd(const int *monitored_fd_set, size_t size);

void handle_connection(int master_socket);

bool is_exit = false;

void server() {
    unlink(SOCKET_NAME);

    int master_socket = system_call_exit_on_failed(socket(AF_UNIX, SOCK_STREAM, 0));
    printf("Master socket created\n");

    struct sockaddr_un addr;
    socket_addr_init(&addr);

    system_call_exit_on_failed(bind(master_socket, (const struct sockaddr *) &addr, sizeof(struct sockaddr_un)));
    printf("Bind succeed\n");

    system_call_exit_on_failed(listen(master_socket, MAX_CLIENT_SUPPORTED));

    int monitored_fd_set[MAX_CLIENT_SUPPORTED] = {0};
    int len_monitored_fd_set = sizeof (monitored_fd_set) / sizeof (int );
    monitored_fd_set_init(monitored_fd_set, len_monitored_fd_set);
    monitored_fd_set_add(monitored_fd_set, len_monitored_fd_set, master_socket);
    fd_set read_fds;

    while (!is_exit) {
        refresh_fd_set(monitored_fd_set, len_monitored_fd_set, &read_fds);

        system_call_exit_on_failed(select(get_max_fd(monitored_fd_set, len_monitored_fd_set) + 1, &read_fds, NULL, NULL, NULL));

        if (FD_ISSET(master_socket, &read_fds)) {
            handle_connection(master_socket);
        }
    }

    close(master_socket);
    printf("Server down.\n");
    unlink(SOCKET_NAME);
    exit(EXIT_SUCCESS);
}

int main() {


    pthread_t tid;
    int ret = pthread_create(&tid, NULL, (void *(*)(void *)) server, NULL);
    if (ret != 0) {
        printf("Create thread failed, error code: %d\n", ret);
        exit(EXIT_FAILURE);
    }


    char buf[BUFSIZ];
    while (fgets(buf, BUFSIZ, stdin)) {
        char cmd[128];
        sscanf(buf, "%s", cmd);
        if (string_equal_ignore_case(cmd, "CREATE")) {
            printf("Create...\n");
        } else if (string_equal_ignore_case(cmd, "UPDATE")) {
            printf("Update...\n");
        } else if (string_equal_ignore_case(cmd, "DELETE")) {
            printf("Deleting...\n");
        } else {
            printf("I have no idea what you are talking about\n");
        }
    }
    is_exit = true;
    pthread_join(tid, NULL);
    return EXIT_SUCCESS;
}

void handle_connection(int master_socket) {
    // accept
    // add_to_monitored_fd_set
    // send multi create command to client to recreate current route table
}

int get_max_fd(const int *monitored_fd_set, size_t size) {
    int max = -1;
    for (int i = 0; i < size; i++) {
        if (monitored_fd_set[i] > max) {
            max = monitored_fd_set[i];
        }
    }
    return max;
}

void refresh_fd_set(const int *monitored_fd_set, size_t size, fd_set *fd_set_ptr) {
    FD_ZERO(fd_set_ptr);
    for (int i = 0; i < size; i++) {
        if (monitored_fd_set[i] != -1) {
            FD_SET(monitored_fd_set[i], fd_set_ptr);
        }
    }
}

bool string_equal_ignore_case(const char *s1, const char *s2) {
    printf("s1: %s, len: %ld\n", s1, strlen(s1));
    printf("s2: %s, len: %ld\n", s2, strlen(s2));
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

void monitored_fd_set_add(int *monitored_fd_set, size_t size, int socket_fd) {
    for (int i = 0; i < size; ++i) {
        if (monitored_fd_set[i] == -1) {
            monitored_fd_set[i] = socket_fd;
            break;
        }
    }
}

void monitored_fd_set_init(int *monitored_fd_set, size_t size) {
//    memset(monitored_fd_set, 0xff, size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        monitored_fd_set[i] = -1;
    }
}

int system_call_exit_on_failed(int return_value) {
    if (return_value == -1) {
        perror("");
        exit(EXIT_FAILURE);
    }
    return return_value;
}

void socket_addr_init(struct sockaddr_un *addr) {
    memset(addr, 0, sizeof(struct sockaddr_un));
    addr->sun_family = AF_UNIX;
    strncpy(addr->sun_path, SOCKET_NAME, sizeof(addr->sun_path) - 1);
}