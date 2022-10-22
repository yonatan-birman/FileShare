#include "server.h"
#include "client.h"
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")
void initialise_winsock();

int main() {
	initialise_winsock();

	SOCKET udp_sockfd = creat_udp_server();
	SOCKET tcp_sockfd = creat_tcp_server();

	// Starting client at a new thread
	HANDLE thread = CreateThread(NULL, 0, start_client, NULL, 0, NULL);
	// Starting server at the current thread
	start_server(udp_sockfd, tcp_sockfd);
}


void initialise_winsock() {
	WSADATA wsa;
	// Initialise winsock
	//printf_s("DEBUG: Initialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		fprintf_s(stderr, "Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	//printf_s("DEBUG: Initialised.\n");
}