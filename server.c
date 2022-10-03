#include "server.h"
// Driver function

void start_server(SOCKET udp_sockfd, SOCKET tcp_sockfd) {


	HANDLE thread = CreateThread(NULL, 0, handle_udp_clients, udp_sockfd, 0, NULL);
	//handle_udp_clients(udp_sockfd);

	handle_tcp_clients(tcp_sockfd);
}


int creat_udp_server() {
	int sockfd;
	struct sockaddr_in servaddr;
	
	// Creating socket file descriptor 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&servaddr, 0, sizeof(servaddr));

	// Filling server information 
	servaddr.sin_family = AF_INET; // IPv4 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(UDP_PORT);

	// Bind the socket with the server address 
	if (bind(sockfd, (const struct sockaddr*) & servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	//printf("bind succided\n");
	return sockfd;
}

int creat_tcp_server()
{
	SOCKET sockfd;
	struct sockaddr_in server;
	int c;

	//Create a socket
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	//printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(TCP_PORT);

	//Bind
	if (bind(sockfd, (struct sockaddr*) & server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	//puts("Bind done");

	//Listen to incoming connections
	listen(sockfd, 3);

	return sockfd;
}
