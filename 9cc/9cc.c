#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if(argc != 2) {
		printf("expected 2 arguments but got %d\n", argc);
		return 1;
	}

	char *p = argv[1];

	printf("\t.intel_syntax noprefix\n");
	printf("\t.globl _main\n");
	printf("_main:\n");
	printf("\tmov rax, %d\n", strtol(p, &p, 10));

	while(*p) {
		if(*p == '+') {
			p++;
			printf("\tadd rax, %d\n", strtol(p, &p, 10));
			continue;
		}
		if(*p == '-') {
			p++;
			printf("\tsub rax, %d\n", strtol(p, &p, 10));
			continue;
		}

		fprintf(stderr, "unexpected token '%c'\n", *p);
		return 1;
	}

	printf("\tret\n");
	return 0;
}
