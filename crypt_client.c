/*
Maze Client
Author: me
Description: Navigate in the maze and don't get caught
*/


#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   

int main(argc, char *argv[])
{
    #ifdef _WIN32
        WSADATA data;
    #endif

        int server_socket;
        struct sockaddr_in 
}
