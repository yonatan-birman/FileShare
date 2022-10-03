#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <Winsock2.h>
#include <WS2tcpip.h>
#include "global.h"
#define size 1024
#define UDP_PORT 8001
#define TCP_PORT 8002
void send_file(char ip[], char path[]);
void scan_for_servers(SOCKET sockfd);
SOCKET creat_client_udp_socket();
SOCKET creat_client_tcp_socket();
void WINAPI start_client();
int nspace_index(const char* str, int n);