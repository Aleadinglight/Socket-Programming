// This UDP server will take command argument as a port number to receive 
// datagrams from clients, and the server bounce back the information with
// server data 

#include <stdio.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: %s registered_port \n", argv[0]);
		return 1;
	}
	// to initialize Winsock API
	WSADATA wsa;
	int r = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (r != 0) {
		printf("Winsock API not ready\n");
		return 1;
	}
	printf("Winsock API ready\n");
	//now we can make socket 
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET) {
		printf("Socket failed\n"); return 1;
	}
	printf("Socket ready\n");
	//use socket for communication
	closesocket(s);
	WSACleanup();
	return 0;
}