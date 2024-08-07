#include <unistd.h>
#include <stdio.h>

int main() {
	char *path = "hello/test";

	if (access(path, F_OK) == 0) {
		printf("File exists\n");
	} else {
		printf("File does not exist\n");
	}

	return 0;
}