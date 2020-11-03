// This UDP server will take command argument as a port number to receive 
// datagrams from clients, and the server bounce back the information with
// server data 

#include <stdio.h>
#include <winsock2.h>
// Link build environmeent to Winsock library
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: %s registered_port \n", argv[0]);
		return 1;
	}
	// Initialize Winsock API
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}
	printf("Winsock API ready\n");
	//now we can make socket 
	SOCKET server = socket(AF_INET, SOCK_DGRAM, 0);
	if (server == INVALID_SOCKET) {
		printf("Socket failed\n"); return 1;
	}
	printf("Socket ready\n");
	//use socket for communication
	closesocket(server);
	WSACleanup();
	return 0;
}