#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if(argc != 2) {
		printf("expected 2 arguments but got %d\n", argc);
		return 1;
	}

	printf("\t.intel_syntax noprefix\n");
	printf("\t.globl _main\n");

	printf("_main:\n");
	printf("\tmov rax, %d\n", atoi(argv[1]));
	printf("\tret\n");
	return 0;
}
