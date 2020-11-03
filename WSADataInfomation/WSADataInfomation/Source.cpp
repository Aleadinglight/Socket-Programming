#include <stdio.h>
#include <winsock2.h>
// Link build environmeent to Winsock library
#pragma comment(lib, "Ws2_32.lib")

int main() {
	// Initialize Winsock API
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}
	printf("Winsock API ready:\n");
	// WORD is unsigned int
	// LOBYTE and HIBYTE
	printf("\Expected version: %u.%u\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));
	printf("\Highest version: %u.%u\n", LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));
	WSACleanup();
	return 0;
}