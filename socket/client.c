#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int main(void) {
	int sock = 0;
	int valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[1024] = {0};

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket creation error\n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 address from test to binary form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("Invalid address, address not supported\n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("Connection Failed\n");
		return -1;
	}
	send(sock, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);
	return 0;
}
