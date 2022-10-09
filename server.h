#pragma once
#include <Winsock2.h>
#define SA struct sockaddr
#define UDP_PORT 8001
#define TCP_PORT 8002

int creat_udp_server();
int creat_tcp_server();
void start_server(SOCKET udp_sockfd, SOCKET tcp_sockfd);
//typedef struct sockaddr sockaddr;
