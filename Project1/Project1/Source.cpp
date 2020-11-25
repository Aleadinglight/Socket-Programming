#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
// Link build environmeent to Winsock library
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
int main() {
	cout << htons(12349);
	return 0;
}