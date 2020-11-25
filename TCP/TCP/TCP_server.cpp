// this tcp server will take command argument for a port number (well-known or registered)
// The server replies to client with server information

#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>

void tcp_server(SOCKET s, short int p);

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: %s [registered_port]\n", argv[0]); return 1;
	}
	WSADATA w;
	int r = WSAStartup(MAKEWORD(2, 2), &w);
	if (r != 0) {
		printf("Winsock API not ready\n"); return 1;
	}
	printf("Winsock API ready\n");
	SOCKET ls = socket(AF_INET, SOCK_STREAM, 0);		// create a listening socket
	if (ls == INVALID_SOCKET) {
		printf("Socket cannot be created\n"); return 1;
	}
	printf("Listening socket is ready\n");
	tcp_server(ls, atoi(argv[1]));
	WSACleanup();
	return 0;
}

void tcp_server(SOCKET ls, short int port) {
	// prepare local socket address for binding
	SOCKADDR_IN sa;
	int salen = sizeof(sa);
	sa.sin_family = AF_INET;			// the address family is set as IPv4
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(port);
	int r = bind(ls, (SOCKADDR*)& sa, salen);
	if (r == SOCKET_ERROR) {
		printf("binding failed, please check the availability of the port\n"); return;
	}
	printf("bind Okay\n");
	r = listen(ls, 5);		// set the socket for incoming connections
	if (r == SOCKET_ERROR) {
		printf("listen failed. \n"); return;
	}
	printf("TCP server is listening on port %d\n", port);
	while (1) {		// this loop runs forever for incoming connections
		SOCKADDR_IN ca;
		int calen = sizeof(ca);
		// ONLY in accept() function, a TCP server can get client address
		SOCKET cs = accept(ls, (SOCKADDR*)& ca, &calen);
		if (cs != SOCKET_ERROR) {
			printf("%s:%d comes in\n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port)); break;
		}
	}
	closesocket(ls);
}