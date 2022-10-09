#pragma once
#include <WinSock2.h>
#define size 1024
#define MSG_CONFIRM 0 //temporary - should fix this

void WINAPI handle_udp_clients(SOCKET sockfd);
void WINAPI handle_tcp_clients(SOCKET sockfd);
