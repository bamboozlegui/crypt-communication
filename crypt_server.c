/*

Author: me
Description: Navigate in the maze and don't get caught

If compiling on Windows link -lws2_32
*/


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
#include <time.h>

#define KEY_LENGTH 9
#define MAX_PASS_LENGTH 256
#define BUFF_LENGTH 1024

char* generate_pass(int n, char* pass);
void parse_buffer(char* buffer, int* pass_length, char key[]);

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

    SOCKET listen_socket;
    SOCKET accept_socket;
    unsigned int port;

    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    char buffer[BUFF_LENGTH];
    char key[9];
    int pass_length;
    char pass[256];
    memset(key, 0, sizeof(key));
    
    socklen_t client_addr_length = sizeof(struct sockaddr);

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

    // create server socket
    printf("Creating socket...\n");
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!IS_VALID_SOCKET(listen_socket))
    {
        fprintf(stderr, "Failed while creating socket. Error: %d\n", GET_SOCKET_ERR());
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
        fprintf(stderr, "Failed while binding socket. Error: %d", GET_SOCKET_ERR());
        exit(1);
    }
    printf("Binded!\n");

    printf("Start listening...\n");
    listen(listen_socket, 5);
    if (!IS_VALID_SOCKET(listen_socket))
    {
        fprintf(stderr, "Failed while creating listen socket. Error: %d\n", GET_SOCKET_ERR());
    }
    printf("Start accepting...\n");
    int r = 0;
    for(;;)
    {
        // clear buffer and client address struct
        memset(&client_address, 0, sizeof(client_address));
        memset(&buffer, 0, BUFF_LENGTH);

        // wait for client to connect
        accept_socket = accept(listen_socket, (struct sockaddr*)&client_address, &client_addr_length);
        if(!IS_VALID_SOCKET(accept_socket))
        {
            fprintf(stderr, "Failed while accepting socket. Error: %s\n", strerror(GET_SOCKET_ERR()));
            exit(-1);
        }
        
        // get key
        r = recv(accept_socket, buffer, sizeof(buffer), 0);
        printf("Received buffer: %s\n", buffer);

        // send data to client
        send(accept_socket, buffer, strlen(buffer), 0);
        CLOSE_SOCKET(accept_socket);
    }

    return 1;
}

char* generate_pass(int n, char* pass)
{
    
    char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{};':\"\\|,.<>/?`~";
    //char pass[n + 1];
    srand(time(NULL)); // seed for pseudo-random number generator
    
    for (int i = 0; i < n; ++i)
    {
        pass[i] = chars[rand() % sizeof(chars) - 1]; // select random character
    }
    pass[n] = '\0';

    return pass;
}

char* encrypt_message(char* message, int length)
{
    for (int i = 0; i < length; i++)
    {
        message[0] += 2;
    }
}

void parse_buffer(char* buffer, int* pass_length, char key[])
{
    char* token = strtok(buffer, " ");

    *pass_length = atoi(token);
    token = strtok(NULL, " ");
    key = token;
    printf("%s", key);
    token = strtok(NULL, " ");
}