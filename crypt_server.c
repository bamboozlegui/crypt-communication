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
#define CLOSE_SOCKET(s) closesocket(s)
#else
#define SOCKET int
#define IS_VALID_SOCKET(s) ((s) >= 0)
#define GET_SOCKET_ERR() (errno)
#define CLOSE_SOCKET(s) close(s)
#endif

    SOCKET listen_socket;
    SOCKET accept_socket;
    unsigned int port;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    char buffer[1024];

    int c_address_length;

    int pub_key;
    int priv_key;

    printf("Validating port...\n");
    if (argc != 2)
    {
        printf("Missing arguments: %s <port>\n", argv[0]);
        exit(-1);
    }
    printf("Port validated!\n");

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
    printf("Successful!\n");
#endif

    // create server socket
    printf("Creating socket...\n");
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!IS_VALID_SOCKET(listen_socket))
    {
        fprintf(stderr, "Failed in socket creation. Error: %d\n", GET_SOCKET_ERR());
    }
    printf("Successful!\n");

    // clear socket memory and fill with addr struct
    memset(&server_address, 0, sizeof(server_address));
    // set protocol
    server_address.sin_family = AF_INET;
    // set address (all network interfaces)
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    // set port
    server_address.sin_port = htons(port);

    printf("Binding to socket...\n");
    if (bind(listen_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        fprintf(stderr, "Failed in binding socket. Error: ", GET_SOCKET_ERR());
        exit(1);
    }
    printf("Binded!\n");

    

    return 1;
}



/*
TO-DO
    INVALID_SOCKET is only on socket() call for Windows. Fix error check logic
    other functions should return SOCKET_ERROR(-1), unix also >0. Use that
    WSAGetLastError() returns both INVALID_SOCKET && SOCKET_ERROR specific error codes
    need to revamp logic for that

*/