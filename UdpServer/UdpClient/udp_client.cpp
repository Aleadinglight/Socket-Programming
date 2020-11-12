// This UDP server will take command argument as a port number to receive 
// datagrams from clients, and the server bounce back the information with
// server data 
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
// Link build environmeent to Winsock library
#pragma comment(lib, "Ws2_32.lib")

void udp_client(SOCKET s, char* ipv4, unsigned short p);

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Usage: %s serverIP serverPort \n", argv[0]);
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
	SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		printf("Socket failed\n"); return 1;
	}
	printf("Socket ready\n");
	//use socket for communication
	udp_client(clientSocket, argv[1], atoi(argv[2]));
	closesocket(clientSocket);
	WSACleanup();
	return 0;
}

void udp_client(SOCKET s, char* ipv4, unsigned short port) {
	SOCKADDR_IN remoteAddress;
	// For ipv4
	remoteAddress.sin_family = AF_INET;
	remoteAddress.sin_addr.s_addr = inet_addr(ipv4);
	remoteAddress.sin_port = htons(port);
	printf("UDP client is ready to send.\nType messages (QUIT will exit)\n");
	while (1) {
		char sbuf[100], rbuf[130];
		memset(sbuf, 0, sizeof(sbuf));
		int sizeOfRemoteAddress = sizeof(remoteAddress);
		printf("Message >> ");
		fgets(sbuf, sizeof(sbuf), stdin);
		if (strcmp(sbuf, "QUIT\n") == 0)
			break;
		// This is UDP so there is no connection established, only datagram is sent
		int result = sendto(s, sbuf, strlen(sbuf), 0, (SOCKADDR *) &remoteAddress, sizeof(remoteAddress));
		// prepare for receiving
		if (result == SOCKET_ERROR) {
			printf("Send failed");
		}
		// Check the server
		SOCKADDR_IN toBeCheckAddress;
		int toBeCheckAddressSize = sizeof(toBeCheckAddress);
		result = recvfrom(s, rbuf, sizeof(rbuf), 0, (SOCKADDR *) & toBeCheckAddress, &toBeCheckAddressSize);
		
		printf("Message from server: %s\n", rbuf);
	}
}