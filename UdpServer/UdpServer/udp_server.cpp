/* This UDP server will take command argument as a port number to create a server at the following port, 
	then receive datagrams from clients, and the server bounce back the information with
	server data */

// Advoid CRT Secure and Deprecated Winsock warning
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
// Link build environment to Winsock library
#pragma comment(lib, "Ws2_32.lib")

void udp_server(SOCKET serverSocket, unsigned short p);

int main(int argc, char** argv) {
	// Check input argument for the port
	if (argc != 2) {
		printf("Usage: %s registered_port \n", argv[0]);
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
	SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
	// If fail to create socket then exit
	if (serverSocket == INVALID_SOCKET) {
		printf("Socket failed\n"); 
		return 1;
	}
	printf("Socket ready\n");
	// Now we create the server and use the socket for communication
	udp_server(serverSocket, atoi(argv[1]));
	// Close socket after used
	closesocket(serverSocket);
	// Clean up 
	WSACleanup();
	return 0;
}

void udp_server(SOCKET serverSocket, unsigned short port) {
	/* struct sockaddr_in {
			short   sin_family;
			u_short sin_port;
			struct  in_addr sin_addr;
			char    sin_zero[8];
		};
	*/
	// This is the local address for the socket
	SOCKADDR_IN sa;
	// Address family is IPv4
	sa.sin_family = AF_INET;
	// Assign IP address to server
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	// Run netstat -an | find "UDP" to see unavailable ports
	// Convert host to network order (Endianness)
	sa.sin_port = htons(port);
	// Bind the socket with the information above
	int result = bind(serverSocket, (SOCKADDR*)& sa, sizeof(sa));
	// If bind failed then close socket and exit
	if (result == SOCKET_ERROR) {
		printf("Bind failed, please check if the port is available\n");
		closesocket(serverSocket);
		return;
	}
	// If the binding work then we start listening on the server
	printf("Bind ok with port number %d\n", port);
	while (1) {
		/* For receiving and sending:
			- rbuf[100]: the buffer for receiving message,
			- sbuf[130]: the buffer for sending message,
			- hname[30]: the array for containing the hostname of the server
		*/
		char rbuf[100], hname[30], sbuf[130];
		// Initialize receiving buffer
		memset(rbuf, 0, sizeof(rbuf));
		// Create a variable for senderAddress
		SOCKADDR_IN senderAddress;
		int senderAddressSize = sizeof(senderAddress);
		// Receiving datagram
		result = recvfrom(serverSocket, rbuf, sizeof(rbuf), 0, (SOCKADDR *) &senderAddress, &senderAddressSize);
		/* If there is data then print out:
			- Sender's address,
			- Sender's port,
			- Sender's message
		*/
		printf("Received from: %s:%d> %s\n", inet_ntoa(senderAddress.sin_addr), ntohs(senderAddress.sin_port), rbuf);
		// Prepare the bounce back message with server's hostname
		gethostname(hname, sizeof(hname));
		sprintf(sbuf, "%s >>> %s", hname, rbuf);
		// Bounce back the message to client with server's data
		result = sendto(serverSocket, sbuf, strlen(sbuf), 0, (SOCKADDR*) &senderAddress, senderAddressSize);
	}
	closesocket(serverSocket);
}