#include "server.h"
#include "client.h"
#pragma comment(lib,"ws2_32.lib")
void initialiseWinsock();

int main() {
	initialiseWinsock();

	SOCKET udp_sockfd = creat_udp_server();
	SOCKET tcp_sockfd = creat_tcp_server();

	//start_client();
	HANDLE thread = CreateThread(NULL, 0, start_client, NULL, 0, NULL);
	start_server(udp_sockfd, tcp_sockfd);
}


void initialiseWinsock() {
	WSADATA wsa;
	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");
}