#include "client.h"
//#include <iostream>
//#pragma comment(lib,"ws2_32.lib") //Winsock Library
//#define _WINSOCK_DEPRECATED_NO_WARNINGS

void WINAPI start_client() {
	const char* scan = "scan", *scan_desc = "scan for live servers";//is it like char[]?
	const char* send = "send ", *send_desc = "send [ip] [file path]";
	const char* end_program = "exit", *end_program_desc = "end program";
	const char* help = "help", *help_desc = "show options";

	int space2_index;


	SOCKET udp_sockfd = creat_client_udp_socket();
	char com[100] = { 0 };
	server_mode = 0;

	while (1) {
		if (!server_mode) {		
			memset(com, 0, sizeof(com));
			printf_s("enter commnad: ");
			//scanf_s("%sockfd", com, _countof(com));//does it matter if i dont write "%9s"?
			fgets(com, sizeof(com), stdin);
			com[strlen(com) - 1] = '\0';
			if (strcmp(com, end_program) == 0) {
				break;
			}
			if (strcmp(com, scan) == 0) {
				scan_for_servers(udp_sockfd);
			}
			else if (strncmp(com, send, 5) == 0) {
				puts("trying to send file");
				space2_index = nspace_index(com, 2);
				com[space2_index] = '\0';
				send_file(&com[5], &com[space2_index + 1]);
			}
			else if (strcmp(com, help) == 0) {
				printf_s("options:\n%s - %s\n%s - %s\n%s - %s\n%s - %s\n",
					scan, scan_desc, send, send_desc, end_program, end_program_desc, help, help_desc);
			}
			//printf_s("command: %s.\n", com);
		}
	}
	closesocket(udp_sockfd);
	exit(0);
}

int nspace_index(const char* str, int n) {//should i use char[] maby?
	int count = 0;
	for (size_t i = 0; i < strlen(str); i++) {//should i make in var len or not?
		if (isspace(str[i])) {
			count++;
			if (count == n) {
				return i;
			}
		}

	}
}

SOCKET creat_client_tcp_socket() {
	SOCKET s;
	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");
	return s;
}

void send_file(char ip[], char path[]) {
	FILE* ptr = NULL;
	char c = 0;
	struct sockaddr_in server;
	char buffer[size];
	int recv_size;
	int i;

	SOCKET sockfd = creat_client_tcp_socket();
	server.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &server.sin_addr);
	server.sin_port = htons(TCP_PORT);

	//Connect to remote server
	if (connect(sockfd, (struct sockaddr*) & server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");

	//Send some data

	// Opening file in reading mode
	fopen_s(&ptr, path, "r");

	if (NULL == ptr) {
		printf_s("file: \"%s\" can't be opened \n", path);
		return;
	}

	printf("content of this file: \n");

	// Printing what is written in file
	// character by character using loop.
	for (i = 0; c != EOF && i < size - 1; i++) {
		c = fgetc(ptr);
		printf("%c", c);
		buffer[i] = c;
	}
	puts("");
	buffer[i] = '\0';
	// Closing the file
	fclose(ptr);

	if (send(sockfd, buffer, strlen(buffer), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Sent");

	//Receive a reply from the server
	if ((recv_size = recv(sockfd, buffer, size - 1, 0)) == SOCKET_ERROR)
	{
		puts("recv failed");
	}
	puts("Reply received:");

	//Add a NULL terminating character to make it a proper string before printing
	buffer[recv_size] = '\0';
	puts(buffer);
	closesocket(sockfd);

}

SOCKET creat_client_udp_socket() {
	SOCKET sockfd;
	struct timeval tv;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	else printf("socket: %d\n", sockfd);


	tv.tv_sec = 1;//why isnt it realy a second?
	tv.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
		perror("Error");
	}
	return sockfd;
}

void scan_for_servers(SOCKET sockfd) {
	const char* search_massage = "someone here?";
	const char* answer_massage = "im here!";
	int len, n;
	char buffer[size];
	struct sockaddr_in broadcastaddr;

	ZeroMemory(&broadcastaddr, sizeof(broadcastaddr));
	broadcastaddr.sin_family = AF_INET;
	broadcastaddr.sin_port = htons(UDP_PORT);
	inet_pton(AF_INET, "192.168.1.255", &broadcastaddr.sin_addr);

	int sendOk = sendto(sockfd, search_massage, strlen(search_massage) + 1, 0, (struct sockaddr*) & broadcastaddr, sizeof(broadcastaddr));
	if (sendOk == SOCKET_ERROR) {
		printf("that didnt work: %d\n", WSAGetLastError());
	}

	while (1)
	{
		len = sizeof(broadcastaddr);
		ZeroMemory(buffer, size);
		ZeroMemory(&broadcastaddr, sizeof(broadcastaddr));
		n = recvfrom(sockfd, (char*)buffer, sizeof(buffer) - 1, /*MSG_WAITALL*/ /*MSG_PEEK*/ 0, (struct sockaddr*) & broadcastaddr, &len);//is it ok sizeof(buffer)-1?
		if (n == -1) {
			perror("Recvfrom timeout");//how does perror work??
			return 1;
		}
		buffer[n] = '\0';
		if (strcmp(buffer, answer_massage) == 0) {
			char tmp[30];
			inet_ntop(AF_INET, &(broadcastaddr.sin_addr), tmp, INET_ADDRSTRLEN);
			printf("server alive at: %s\n", tmp);
			//add tmp to some kind of a list
		}
		else
			printf("%s", buffer);
	}
}