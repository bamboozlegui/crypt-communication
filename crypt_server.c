/*

Author: me
Description: Navigate in the maze and don't get caught
*/


#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") // need this - Winsock library
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

int main(int argc, char* argv[])
{
#ifdef _WIN32
    WSADATA wsa;
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define GETSOCKETERRNO() (WSAGetLastError())
#else
    SOCKET int // Winsock uses SOCKET object
    ISVALIDSOCKET(s) ((s) >= 0)
    GETSOCKETERRNO() (errno)
#endif

    SOCKET listen_socket;
    SOCKET accept_socket;
    unsigned int port;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;

    int c_address_length;

    printf("Validating port...");
    if (argc != 2)
    {
        printf("Missing arguments: %s <port>");
        exit(-1);
    }

    if (port < 1 || port > 65535)
    {
        printf("Port number out of range. Allowed: [1,65535]");
        exit(-1);
    }

#ifdef _WIN32
    printf("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Unsuccessful. Error: $d", WSAGetLastError());
        exit(-1);
    }
    printf("Successful.");
#endif

    // create server socket
    if ((listen_socket = socket(AF_INET, SOCK_STREAM, 0))) // SOCK_STREAM - connection oriented TCP protocol
    {
        ;
    }
    return 1;
}
