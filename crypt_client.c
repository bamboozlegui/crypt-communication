/*

Author: me
Description: Navigate in the maze and don't get caught
*/


#ifdef _WIN32
#include <winsock2.h>
#define socklen_t int
#define IS_VALID_SOCKET(s) ((s) != INVALID_SOCKET)
#define GET_SOCKET_ERR() (WSAGetLastError())
#define CLOSE_SOCKET(s) closesocket(s)
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#define SOCKET int
#define IS_VALID_SOCKET(s) ((s) >= 0)
#define GET_SOCKET_ERR() (errno)
#define CLOSE_SOCKET(s) close(s)
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  

int main(int argc, char* argv[])
{
#ifdef _WIN32
	WSADATA wsa;

    printf("Initialising Winsock... \n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Unsuccessful. Error: %d\n", WSAGetLastError());
        exit(-1);
    }
    printf("Successful!\n");
#endif

	unsigned short port;
    SOCKET listen_socket;
    struct sockaddr_in server_address;
    char buffer[1024];

    if (argc != 3) {
        fprintf(stderr, "Missing arguments: %s <ip> <port>", argv[0]);
        exit(-1);
    }

    port atoi(argv[2]);

    if (port < 1 || port > 65535)
    {
        printf("Port number out of range. Allowed: [1,65535]");
        exit(-1);
    }

    // create socket
    printf("Creating socket...\n");
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!IS_VALID_SOCKET(listen_socket))
    {
        fprintf(stderr, "Failed while creating socket. Error: %d\n", GET_SOCKET_ERR());
    }
    printf("Successful!\n");


	
	return 1;
}