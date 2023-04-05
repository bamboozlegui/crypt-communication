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

int main(int argc, char* argc[])
{
	return 1;
}