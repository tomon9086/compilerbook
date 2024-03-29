#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
	TK_RESERVED,
	TK_NUMBER,
	TK_EOF
} TokenKind;

typedef struct Token Token;

struct Token {
	TokenKind kind;
	Token *next;
	int val;
	char *str;
};

char *user_input;
Token *token;

void error(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

void error_at(char *loc, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, "");
	fprintf(stderr, "^ ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

bool consume(char op) {
	if(token->kind != TK_RESERVED || token->str[0] != op) {
		return false;
	}
	token = token->next;
	return true;
}

void expect(char op) {
	if(token->kind != TK_RESERVED || token->str[0] != op) {
		error_at(token->str, "unexpected token '%c'", op);
	}
	token = token->next;
}

int expect_number() {
	if(token->kind != TK_NUMBER) {
		error_at(token->str, "not a number");
	}
	int val = token->val;
	token = token->next;
	return val;
}

bool at_eof() {
	return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *curr, char *str) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	curr->next = tok;
	return tok;
}

Token *tokenize(char *p) {
	Token head;
	head.next = NULL;
	Token *curr = &head;

	while(*p) {
		if(isspace(*p)) {
			p++;
			continue;
		}
		if(*p == '+' || *p == '-') {
			curr = new_token(TK_RESERVED, curr, p++);
			continue;
		}
		if(isdigit(*p)) {
			curr = new_token(TK_NUMBER, curr, p);
			curr->val = strtol(p, &p, 10);
			continue;
		}

		error_at(token->str, "cannot be tokenized");
	}
	new_token(TK_EOF, curr, p);
	return head.next;
}

int main(int argc, char **argv) {
	if(argc != 2) {
		printf("expected 2 arguments but got %d\n", argc);
		return 1;
	}

	user_input = argv[1];
	token = tokenize(user_input);

	printf("\t.intel_syntax noprefix\n");
	printf("\t.globl _main\n");
	printf("_main:\n");

	printf("\tmov rax, %d\n", expect_number());

	while(!at_eof()) {
		if(consume('+')) {
			printf("\tadd rax, %d\n", expect_number());
			continue;
		}

		expect('-');
		printf("\tsub rax, %d\n", expect_number());
	}

	printf("\tret\n");
	return 0;
}
