/*

Author: me
Description: Navigate in the maze and don't get caught

If compiling on Windows link -lws2_32
*/


#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>   

int main(int argc, char* argv[])
{
#ifdef _WIN32
    WSADATA wsa;
#define IS_VALID_SOCKET(s) ((s) != INVALID_SOCKET)
#define GET_SOCKET_ERR() (WSAGetLastError())
#else
#define SOCKET int
#define IS_VALID_SOCKET(s) ((s) >= 0)
#define GET_SOCKET_ERR() (errno)
#endif

    SOCKET listen_socket;
    SOCKET accept_socket;
    unsigned int port;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    int c_address_length;

    printf("Validating port...\n");
    if (argc != 2)
    {
        printf("Missing arguments: %s <port>", argv[0]);
        exit(-1);
    }
    printf("Port validated.\n");

    port = atoi(argv[1]);

    if (port < 1 || port > 65535)
    {
        printf("Port number out of range. Allowed: [1,65535]");
        exit(-1);
    }

#ifdef _WIN32
    printf("Initialising Winsock... \n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Unsuccessful. Error: %d\n", WSAGetLastError());
        exit(-1);
    }
    printf("Successful.\n");
#endif

    // create server socket
    printf("Creating socket...\n");
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(!IS_VALID_SOCKET(listen_socket))
    {
        fprintf(stderr, "Failed in socket creation: %d\n", GET_SOCKET_ERR());
    }
    printf("Successful.\n");



    return 1;
}
