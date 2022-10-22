#include "client_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WS2tcpip.h>
#include "global.h"

// Answer udp broadcast messages from clients
void WINAPI handle_udp_clients(SOCKET sockfd) {
	struct sockaddr_in cliaddr;
	char clientIp[256];
	char buffer[SIZE];
	int len, n;
	len = sizeof(cliaddr);
	while (1) {
		ZeroMemory(buffer, SIZE);
		//printf("DEBUG: reciving from socket: %d\n", sockfd);
		n = recvfrom(sockfd, (char*)buffer, sizeof(buffer) - 1, /*MSG_WAITALL*/ /*MSG_PEEK*/ 0, (struct sockaddr*) & cliaddr, &len);
		if (n == -1) {
			fprintf_s(stderr, "Recvfrom failed\n");
			exit(1);
		}
		buffer[n] = '\0';
		//printf_s("DEBUG: Client : %s\n", buffer);
		inet_ntop(AF_INET, &cliaddr.sin_addr, &clientIp, sizeof(clientIp));
		if (strcmp(buffer, SEARCH_MESSAGE) == 0) {
			sendto(sockfd, ANSWER_MESSAGE, strlen(ANSWER_MESSAGE), MSG_CONFIRM, (const struct sockaddr*) & cliaddr, len);
			//printf("DEBUG: \"%s\" sent.\n", ANSWER_MESSAGE);
		}
	}
	closesocket(sockfd);
	WSACleanup();
}

// Accept and save files from clients
void WINAPI handle_tcp_clients(SOCKET sockfd) {
	server_mode = 0;
	char buffer[SIZE];
	int recv_size;
	SOCKET new_socket;
	struct sockaddr_in client;
	int len;
	// Accept an incoming connection
	puts("Waiting for incoming files...");
	while (1) {
		char ip[30];
		char ans[30];
		FILE* f;
		len = sizeof(struct sockaddr_in);
		new_socket = accept(sockfd, (struct sockaddr*) & client, &len);
		if (new_socket == INVALID_SOCKET) {
			printf_s("Accept failed with error code : %d", WSAGetLastError());
		}

		//puts("DEBUG: Connection accepted");

		memset(buffer, 0, SIZE);
		if ((recv_size = recv(new_socket, buffer, SIZE, 0)) == SOCKET_ERROR)
		{
			puts("Recv failed");
			exit(0);
		}
		buffer[recv_size] = '\0';
		server_mode = 1;
		send(new_socket, CONFIRM_MSG, strlen(CONFIRM_MSG), 0);
		//need to stop client thread here

		inet_ntop(AF_INET, &(client.sin_addr), ip, INET_ADDRSTRLEN);
		printf_s("\nip: %s sent you a file, press y to save it or n to dismiss: ", ip);
		while (server_mode) {
			//scanf_s("%s", ans, _countof(ans));
			//getchar();
			fgets(ans, sizeof(ans), stdin);
			ans[1] = '\0';
			*ans = tolower(*ans);
			if (strcmp("y", ans) == 0) {
				printf_s("What would you like to name the file? \n");
				scanf_s("%s", ans, _countof(ans));
				if (fopen_s(&f, ans, "w") == 0) {
					fwrite(buffer, sizeof(char), strlen(buffer), f);
					fclose(f);
					server_mode = 0;
				}

			}
			else if (strcmp("n", ans) == 0) {
				puts("OK, file dismissed");
				server_mode = 0;
			}
			else {
				printf_s("%c is not a valid answer, please press y to save the file or n to dismiss: ", *ans);
			}
		}
	}
	closesocket(sockfd);
	WSACleanup();

	return 0;
}
