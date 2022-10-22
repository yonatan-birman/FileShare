#pragma once
#include <WinSock2.h>
#define SIZE 1024
#define MSG_CONFIRM 0
#define SEARCH_MESSAGE "someone here?"
#define ANSWER_MESSAGE "im here!"
#define CONFIRM_MSG "file recived:)"
void WINAPI handle_udp_clients(SOCKET sockfd); // Answer udp broadcast messages from clients
void WINAPI handle_tcp_clients(SOCKET sockfd); // Accept and save files from clients
