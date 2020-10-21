#include <stdio.h>
#include <stdlib.h>


// This testing program will take a dotted-decimal IPv4 address as command-line argument,
// and the programs tells what class this address belongs to

int main(int argc, char **argv) {
	// argc is the number of arguments, including the application command itself
	// argv is the array of argument text, each is a char string
	if (argc != 2) {
		printf("Usage: %s IPv4_addr_dotted-decimal\n", argv[0]);
		return -1;
	}
	printf("Your argument is: %s\n", argv[1]);

	// Analyse the class of this addr, find the first number in this argument 
	for (int i=0; ;i++) {
		if (argv[1][i] == '.') {
			argv[1][i] = '\0';
			break;
		}
	}

	int n = atoi(argv[1]);
	if (n < 128) {
		printf("Class A\n");
	}
	else if (n < 192) {
		 printf("Class B\n");
	}
	else if (n < 224) {
		 printf("Class C\n");
	}
	else {
		printf("Unknown class\n");
	}
	return 0;
}