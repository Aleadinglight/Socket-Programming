// Socket.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <WinSock2.h>

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s IPv4_addr \n", argv[0]);
		return 1;
	}
	printf("Your address is %s\n", argv[1]);

	for (int i = 0; ; i++) {
		if (argv[1][i] == '.') {
			argv[1][i] = '\0'; break;
		}
	}
	printf("Your first part is %s\n", argv[1]);
	int n = atoi(argv[1]);
	if (n < 128) printf("Class A\n");
	else if (n < 192) printf("Class B\n");
	else if (n < 224) printf("Class C\n");
	else printf("Not a unicast\n");
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
