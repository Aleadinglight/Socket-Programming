/* This UDP client will take 2 command argument as an IP address and a port number.
	Then send message and receive data (with checking) from the server.*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
	// Link build environmeent to Winsock library
#pragma comment(lib, "Ws2_32.lib")

void tcp_client(SOCKET s, char* ipv4, unsigned short p);

int main(int argc, char** argv) {
	// Checking arguments
	if (argc != 3) {
		printf("Usage: %s serverIP serverPort \n", argv[0]);
		return 1;
	}
	// Initialize Winsock API
	// The WSAStartup function is called to initiate use of WS2_32.dll
	WSADATA wsaData;
	// MAKEWORD(2, 2) makes a request for version 2.2 of Winsock on the system, 
	// and sets the passed version as the highest version of Windows Sockets support that the caller can use.
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	// If the request failed then exit
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}
	printf("Winsock API ready\n");
	/* Orthewise the request succeeded, now we can create socket
		- AF_INET: Network address family for IPv4
		- SOCK_DGRAM: Connection type, here we use datagram
		- Protocol: Transport layer protocol, here we use 0
	*/
	SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	// If fail to create socket then exit
	if (clientSocket == INVALID_SOCKET) {
		printf("Socket failed\n");
		return 1;
	}
	printf("Socket ready\n");
	// Now we create the server and use the socket for communication
	tcp_client(clientSocket, argv[1], atoi(argv[2]));
	// Close socket after used
	closesocket(clientSocket);
	// Clean up 
	WSACleanup();
	return 0;
}

void tcp_client(SOCKET s, char* ipv4, unsigned short port) {
	/* struct sockaddr_in {
			short   sin_family;
			u_short sin_port;
			struct  in_addr sin_addr;
			char    sin_zero[8];
		};
	*/
	SOCKADDR_IN remoteAddress;
	int sizeOfRemoteAddress = sizeof(remoteAddress);
	// Address family is IPv4
	remoteAddress.sin_family = AF_INET;
	// IP address of server
	remoteAddress.sin_addr.s_addr = inet_addr(ipv4);
	// Port of server
	remoteAddress.sin_port = htons(port);
	int result = connect(s, (SOCKADDR*)& remoteAddress, sizeOfRemoteAddress);
	if (result == SOCKET_ERROR) {
		printf("Failed to connect to server");
		return;
	}
	printf("Connection to server established. Type message (QUIT will exit)\n");
	while (1) {
		char sbuf[100], rbuf[1000];
		memset(sbuf, 0, sizeof(sbuf));
		memset(rbuf, 0, sizeof(rbuf));
		result = recv(s, rbuf, sizeof(rbuf), 0);
		if (result == SOCKET_ERROR) {
			printf("Socket error, stop...\n");
			break;
		}
		printf("Received >> %s\n", rbuf);
		printf("Message >> ");
		fgets(sbuf, sizeof(sbuf), stdin);
		// If user type this then quit
		if (strcmp(sbuf, "QUIT\n") == 0)
			break;
		// This is UDP so there is no connection established, only datagram is sent
		int result = send(s, sbuf, strlen(sbuf), 0);
		// Prepare for receiving
		if (result == SOCKET_ERROR) {
			printf("Send failed");
		}
	}
}