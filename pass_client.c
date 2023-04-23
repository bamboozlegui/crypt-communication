#ifdef _WIN32
#include <winsock2.h>
#define socklen_t int
#define IS_VALID_SOCKET(s) ((s) != (SOCKET_ERROR || INVALID_SOCKET))
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

    SOCKET server_socket;
    unsigned int port;

    struct sockaddr_in server_address;

    char buffer[1024];
    int pass_count;

    if (argc != 3)
    {
        fprintf(stderr, "Missing arguments: %s <ip> <port>", argv[0]);
        exit(-1);
    }

    port = atoi(argv[2]);

    if (port < 1 || port > 65535)
    {
        printf("Port number out of range. Allowed: [1,65535]");
        exit(-1);
    }

    printf("Creating socket...\n");
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!IS_VALID_SOCKET(server_socket))
    {
        fprintf(stderr, "Failed while creating socket. Error: %d\n", GET_SOCKET_ERR());
    }
    printf("Successful!\n");

    // clear socket memory and fill with addr struct
    memset(&server_address, 0, sizeof(server_address));
    // set protocol(IPv4) and port
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

#ifdef _WIN32
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    if (server_address.sin_addr.s_addr == INADDR_NONE) // wrong format address returns INADDR_NONE on winsock2
    {
        fprintf(stderr, "Invalid server IP address.\n");
        exit(-1);
    }
#else
    if (inet_aton(argv[1], &server_address.sin_addr) <= 0) {
        fprintf(stderr, "Invalid server IP address.\n");
        exit(-1);
    }
#endif

    printf("Connecting to server...\n");
    int i_result = connect(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if (!IS_VALID_SOCKET(i_result))
    {
        fprintf(stderr, "Failed while connecting to server: %d\n", GET_SOCKET_ERR());
        exit(-1);
    }

    while (IS_VALID_SOCKET(server_socket) > 0)
    {
    printf("Enter password length and the amount separated by a space (MAX 20 for each value): ");
    fgets(buffer, 1024, stdin);
    // #1 send pass count andl length to server
    send(server_socket, buffer, strlen(buffer), 0);


    printf("Checking input validity...\n");
    // #2 check server response for validity
    recv(server_socket, buffer, sizeof(buffer), 0);
        printf("%s", buffer);

    // #3 get pass count
    recv(server_socket, buffer, sizeof(buffer), 0);
    pass_count = atoi(buffer);

    memset(&buffer, 0, 1024);
    for (int i = 0; i < pass_count; ++i)
    {
        // #4 get passwords
        recv(server_socket, buffer, sizeof(buffer), 0);
        printf("Pass %i: %s\n", i + 1, buffer);
        memset(&buffer, 0, 1024);
    }
        printf("\n%i\n", server_socket);
        CLOSE_SOCKET(server_socket);
        printf("\n%i\n", server_socket);
    }

    CLOSE_SOCKET(server_socket);
 
    return 1;
}
