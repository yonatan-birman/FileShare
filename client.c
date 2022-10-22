#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <WS2tcpip.h>
#include "global.h"
//#pragma comment(lib,"ws2_32.lib") //Winsock Library
//#define _WINSOCK_DEPRECATED_NO_WARNINGS //for debuging purposes

// Start receiving commands from user 
void WINAPI start_client() {
	int space2index;
	SOCKET udp_sockfd = creat_client_udp_socket();
	char com[100] = { 0 };
	server_mode = 0;

	while (1) {
		if (!server_mode) {
			memset(com, 0, sizeof(com));
			printf_s("Command: ");
			fgets(com, sizeof(com), stdin);
			com[strlen(com) - 1] = '\0';
			if (strcmp(com, END_PROGRAM) == 0) {
				break;
			}
			if (strcmp(com, SCAN) == 0) {
				scan_for_servers(udp_sockfd);
			}
			else if (strncmp(com, SEND, 5) == 0) {
				puts("Sending file...");
				space2index = nspace_index(com, 2);
				com[space2index] = '\0';
				send_file(&com[5], &com[space2index + 1]);
			}
			else if (strcmp(com, HELP) == 0) {
				printf_s("Options:\n%s - %s\n%s- %s\n%s - %s\n%s - %s\n",
					SCAN, SCAN_DESC, SEND, SEND_DESC, END_PROGRAM, END_PROGRAM_DESC, HELP, HELP_DESC);
			}
			//printf_s("DEBUG: command: %s.\n", com);
		}
	}
	closesocket(udp_sockfd);
	exit(0);
}

// Return the index of the n space from left to right
int nspace_index(const char* str, int n) {
	int count = 0;
	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++) {
		if (isspace(str[i])) {
			count++;
			if (count == n) {
				return i;
			}
		}

	}
}

// Creat a tcp socket so the client can send files to other computers
SOCKET creat_client_tcp_socket() {
	SOCKET s;
	// Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf_s("Could not create socket : %d", WSAGetLastError());
		exit(1);
	}
	//printf_s("DEBUG: Socket created.\n");
	return s;
}

// Creat a connection with the specified ip address using tcp protocol and send the specified file
void send_file(char ip[], char path[]) {
	FILE* ptr = NULL;
	char c = 0;
	struct sockaddr_in server;
	char buffer[SIZE];
	int recv_size;
	int i;

	SOCKET sockfd = creat_client_tcp_socket();
	server.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &server.sin_addr);
	server.sin_port = htons(TCP_PORT);

	// Connect to remote server
	if (connect(sockfd, (struct sockaddr*) & server, sizeof(server)) < 0) {
		puts("connect error");
		closesocket(sockfd);
		return;
	}
	//puts("DEBUG: Connected");

	// Opening file in reading mode
	fopen_s(&ptr, path, "r");

	if (NULL == ptr) {
		printf_s("File: \"%s\" can't be opened \n", path);
		closesocket(sockfd);
		return;
	}

	printf_s("Content of this file: \n");//for smaller files

	// Printing what is written in file
	// character by character using loop.
	for (i = 0; (c = fgetc(ptr)) != EOF && i < SIZE - 1; i++) {
		printf_s("%c", c);//for smaller files
		buffer[i] = c;
	}
	puts("");
	buffer[i] = '\0';
	// Closing the file
	fclose(ptr);

	if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
		puts("Send failed");
		closesocket(sockfd);
		return;
	}
	puts("Data Sent");

	// Receive a reply from the server
	if ((recv_size = recv(sockfd, buffer, SIZE - 1, 0)) == SOCKET_ERROR) {
		puts("Recv failed");
		closesocket(sockfd);
		return;
	}

	// Add a NULL terminating character to make it a proper string before printing
	buffer[recv_size] = '\0';
	printf_s("Reply from server: %s\n", buffer);

	closesocket(sockfd);

}

// Creat a udp socket so the client can scan the network
SOCKET creat_client_udp_socket() {
	SOCKET sockfd;
	struct timeval tv;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}
	//printf_s("DEBUG: socket: %d\n", sockfd);

	tv.tv_sec = 1;
	tv.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
		perror("Error");
		exit(1);
	}
	return sockfd;
}

// Broadcast a message to the local network so that all the live servers will return answer and we will know who is available
void scan_for_servers(SOCKET sockfd) {
	const char* const SEARCH_MESSAGE = "someone here?";
	const char* const ANSWER_MESSAGE = "im here!";
	int len, n;
	char buffer[SIZE];
	struct sockaddr_in broadcastaddr;

	ZeroMemory(&broadcastaddr, sizeof(broadcastaddr));
	broadcastaddr.sin_family = AF_INET;
	broadcastaddr.sin_port = htons(UDP_PORT);
	inet_pton(AF_INET, "192.168.1.255", &broadcastaddr.sin_addr);//should make this generic

	int sendOk = sendto(sockfd, SEARCH_MESSAGE, strlen(SEARCH_MESSAGE) + 1, 0, (struct sockaddr*) & broadcastaddr, sizeof(broadcastaddr));
	if (sendOk == SOCKET_ERROR) {
		printf_s("That didnt work: %d\n", WSAGetLastError());
	}

	while (1) {
		len = sizeof(broadcastaddr);
		ZeroMemory(buffer, SIZE);
		ZeroMemory(&broadcastaddr, sizeof(broadcastaddr));
		n = recvfrom(sockfd, (char*)buffer, sizeof(buffer) - 1, /*MSG_WAITALL*/ /*MSG_PEEK*/ 0, (struct sockaddr*) & broadcastaddr, &len);
		if (n == -1) {
			//printf_s("DEBUG: Recvfrom timeout");
			return;
		}
		buffer[n] = '\0';
		if (strcmp(buffer, ANSWER_MESSAGE) == 0) {
			char tmp[30];
			inet_ntop(AF_INET, &(broadcastaddr.sin_addr), tmp, INET_ADDRSTRLEN);
			printf_s("Server alive at: %s\n", tmp);
		}
		else
			printf_s("%s", buffer);
	}
}