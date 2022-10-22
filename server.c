#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include "client_handler.h"

// Creating two threads so the server can answer to udp broadcast messages and also receive files using tcp
void start_server(SOCKET udp_sockfd, SOCKET tcp_sockfd) {
	HANDLE thread = CreateThread(NULL, 0, handle_udp_clients, udp_sockfd, 0, NULL);
	handle_tcp_clients(tcp_sockfd);
}

// Creat udp socket bind to port UDP_PORT
int creat_udp_server() {
	int sockfd;
	struct sockaddr_in servaddr;

	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		fprintf_s(stderr, "Socket creation failed\n");
		exit(1);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(UDP_PORT);

	// Bind the socket with the server address 
	if (bind(sockfd, (const struct sockaddr*) & servaddr, sizeof(servaddr)) < 0) {
		fprintf_s(stderr, "Bind failed\n");
		exit(1);
	}
	//puts("DEBUG: bind succided");
	return sockfd;
}

// Creat tcp socket bind to port TCP_PORT
int creat_tcp_server() {
	SOCKET sockfd;
	struct sockaddr_in server;
	int c;

	// Create a socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		fprintf_s(stderr, "Socket creation failed\n");
		exit(1);
	}
	//puts("DEBUG: Socket created");

	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(TCP_PORT);

	// Bind
	if (bind(sockfd, (struct sockaddr*) & server, sizeof(server)) == SOCKET_ERROR) {
		fprintf_s(stderr, "Bind failed");
		exit(1);
	}
	//puts("DEBUG: Bind done");

	// Listen to incoming connections
	listen(sockfd, 1);

	return sockfd;
}
