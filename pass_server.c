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

#define MAX_COUNT 20
#define BUFF_LENGTH 1024

char* generate_pass(int n, char* pass);
void parse_buffer(char* buffer, int* pass_length, int* pass_count);

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
    
    socklen_t client_addr_length = sizeof(struct sockaddr);

    char buffer[BUFF_LENGTH];
    int pass_count;
    int pass_length;
    char pass[256];
    srand(time(NULL)); // seed for pseudo-random number generations


    printf("Validating arguments...\n");
    if (argc != 2)
    {
        printf("Missing arguments: %s <port>\n", argv[0]);
        exit(-1);
    }
    printf("Arguments passed!\n");

    port = atoi(argv[1]);

    printf("Validating connection port...\n");
    if (port < 1 || port > 65535)
    {
        printf("Port number out of range. Allowed: [1,65535]");
        exit(-1);
    }
    printf("Connection port validated!\n");

    printf("Creating socket...\n");
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (!IS_VALID_SOCKET(listen_socket))
    {
        fprintf(stderr, "Failed while creating socket. Error: %d\n", GET_SOCKET_ERR());
    }
    printf("Successful!\n");

    // clear socket memory and fill with addr struct
    memset(&server_address, 0, sizeof(server_address));

    // set protocol, address(to all network interfaces), port

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    printf("Binding to socket...\n");
    if (bind(listen_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
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

    for (;;)
    {
        // clear buffer and client address struct
        memset(&client_address, 0, sizeof(client_address));
        memset(&buffer, 0, BUFF_LENGTH);

        // wait for client to connect
        accept_socket = accept(listen_socket, (struct sockaddr*)&client_address, &client_addr_length);
        if (!IS_VALID_SOCKET(accept_socket))
        {
            fprintf(stderr, "Failed while accepting socket. Error: %s\n", strerror(GET_SOCKET_ERR()));
            exit(-1);
        }
        printf("Peer connected.\n");

        // #1 get password lengt and count from client
        recv(accept_socket, buffer, sizeof(buffer), 0);
        printf("Received password length and count: %s\n", buffer);

        parse_buffer(buffer, &pass_length, &pass_count);
        printf("Parsed length and count: %i and %i\n", pass_length, pass_count);

        if (pass_length > 20 || pass_count > 20)
        {
            // #2 send info about password validity
            printf("Password arguments are out of bound.\n");
            strncpy(buffer, "Max value for password length or count exceeded. Length and count are set to 20.\n", 82);
            send(accept_socket, buffer, strlen(buffer), 0);

            pass_length = 20;
            pass_count = 20;
        }
        else
        {
            strncpy(buffer, "Password arguments are valid.\n", 31);
            send(accept_socket, buffer, 31, 0);
        }

        // #3 convert send password count
        sprintf(buffer, "%i", pass_count);
        send(accept_socket, buffer, strlen(buffer), 0);

        printf("Here are the passwords:\n");
        for (int i = 0; i < pass_count; ++i)
        {
            generate_pass(pass_length, pass);
            printf("Generating pass #%i.... %s\n", i+1, pass);
            send(accept_socket, pass, pass_length, 0);
        }

        CLOSE_SOCKET(accept_socket);
        printf("Peer disconnected.\n");
    }

    return 1;
}

// function for generating pseudo-random password

char* generate_pass(int n, char* pass)
{
    char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{};':\"\\|,.<>/?`~";
    for (int i = 0; i < n; ++i)
    {
        pass[i] = chars[rand() % (sizeof(chars) - 1)]; // select random character
    }
    pass[n] = '\0';

    return pass;
}

void parse_buffer(char* buffer, int* pass_length, int* pass_count)
{
    char* token = strtok(buffer, " ");

    *pass_length = atoi(token);
    token = strtok(NULL, " ");
    *pass_count = atoi(token);
}
