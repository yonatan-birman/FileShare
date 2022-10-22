#pragma once
#include <Winsock2.h>
#define SIZE 1024
#define UDP_PORT 8001
#define TCP_PORT 8002

#define SCAN "scan"
#define SCAN_DESC "scan for live servers"
#define SEND "send "
#define SEND_DESC "send [ip] [file path]"
#define END_PROGRAM "exit"
#define END_PROGRAM_DESC "end program"
#define HELP "help"
#define HELP_DESC "show options"
//#define SEARCH_MESSAGE "someone here?"
//#define ANSWER_MESSAGE "im here!"

void send_file(char ip[], char path[]); // Creat a connection with the specified ip address using tcp protocol and send the specified file
void scan_for_servers(SOCKET sockfd); // Broadcast a message to the local network so that all the live servers will return answer and we will know who is available
SOCKET creat_client_udp_socket(); // Creat a udp socket so the client can scan the network
SOCKET creat_client_tcp_socket(); // Creat a tcp socket so the client can send files to other computers
void WINAPI start_client(); // Start receiving commands from user 
int nspace_index(const char* str, int n); // Return the index of the n space from left to right