#pragma once
#include <Winsock2.h>
#define SA struct sockaddr
#define UDP_PORT 8001
#define TCP_PORT 8002

int creat_udp_server(); // Creat udp socket bind to port UDP_PORT
int creat_tcp_server(); // Creat tcp socket bind to port TCP_PORT
void start_server(SOCKET udp_sockfd, SOCKET tcp_sockfd); // Creating two threads so the server can answer to udp broadcast messages and also receive files using tcp

