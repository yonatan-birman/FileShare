#pragma once
//#include "massage.h"
#include <stdio.h>
//#include <netdb.h>
//#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "global.h"
//#include <sys/socket.h>
//#include <sys/types.h>
#define size 1024
#define MSG_CONFIRM 0 //temporary - should fix this
//#define search_massage "someone here?"
//#define answer_massage "im here!"
void WINAPI handle_udp_clients(SOCKET sockfd);
void WINAPI handle_tcp_clients(SOCKET sockfd);
