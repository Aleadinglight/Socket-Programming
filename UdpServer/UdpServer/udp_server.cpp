// This UDP server will take command argument as a port number to receive 
// datagrams from clients, and the server bounce back the information with
// server data 

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
// Link build environmeent to Winsock library
#pragma comment(lib, "Ws2_32.lib")

void udp_server(SOCKET serverSocket, unsigned short p);

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
	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (serverSocket == INVALID_SOCKET) {
		printf("Socket failed\n"); return 1;
	}
	printf("Socket ready\n");
	udp_server(serverSocket, atoi(argv[1]));
	//use socket for communication
	closesocket(serverSocket);
	WSACleanup();
	return 0;
}

void udp_server(SOCKET serverSocket, unsigned short port) {
	/** struct sockaddr_in {
			short   sin_family;
			u_short sin_port;
			struct  in_addr sin_addr;
			char    sin_zero[8];
		};
	**/
	// local address for the socket
	SOCKADDR_IN sa;
	// address family is IPv4
	sa.sin_family = AF_INET;
	// 0.0.0.0
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	// run netstat -an | find "UDP" to see unavailable ports
	// host to network orer (Endianness)
	sa.sin_port = htons(port);
	int result = bind(serverSocket, (SOCKADDR*)& sa, sizeof(sa));
	if (result == SOCKET_ERROR) {
		printf("Bind failed, please check if the port is available\n");
		closesocket(serverSocket);
		return;
	}

	printf("Bind ok with port number %d\n", port);
	while (1) {
		// for receiving and sending
		char rbuf[100], hname[30], sbuf[130];
		memset(rbuf, 0, sizeof(rbuf));
		SOCKADDR_IN serverAddress;
		int serverAddressSize = sizeof(serverAddress);
		result = recvfrom(serverSocket, rbuf, sizeof(rbuf), 0, (SOCKADDR *) &serverAddress, &serverAddressSize);
		// for receiving and sending
		printf("Received from: %s:%d> %s\n", inet_ntoa(serverAddress.sin_addr), ntohs(serverAddress.sin_port), rbuf);
		gethostname(hname, sizeof(hname));
		sprintf(sbuf, "%s >>> %s", hname, rbuf);
		result = sendto(serverSocket, sbuf, strlen(sbuf), 0, (SOCKADDR*) &serverAddress, serverAddressSize);
	}
	closesocket(serverSocket);
}