#include <stdio.h>
#include "tokens.h"

const char * token_get_symbols(enum TOKENS token)
{
	switch(token) {
		case TOKEN_COMMA: return ",";
		case TOKEN_COLON: return ":";
		case TOKEN_SEMICOLON: return ";";
		case TOKEN_LEFT_PARENTHESIS: return "(";
		case TOKEN_RIGHT_PARENTHESIS: return ")";
		case TOKEN_LEFT_BRACKET: return "[";
		case TOKEN_RIGHT_BRACKET: return "]";
		case TOKEN_LEFT_BRACE: return "{";
		case TOKEN_RIGHT_BRACE: return "}";
		case TOKEN_LOGICAL_NOT: return "!";
		case TOKEN_BITWISE_NOT: return "~";
		case TOKEN_ADD: return "+";
		case TOKEN_SUB: return "-";
		case TOKEN_MUL: return "*";
		case TOKEN_DIV: return "/";
		case TOKEN_MOD: return "%";
		case TOKEN_XOR: return "^";
		case TOKEN_BITWISE_AND: return "&";
		case TOKEN_BITWISE_OR: return "|";
		case TOKEN_LOGICAL_AND: return "&&";
		case TOKEN_LOGICAL_OR: return "||";
		case TOKEN_LT: return "<";
		case TOKEN_GT: return ">";
		case TOKEN_LTE: return "<=";
		case TOKEN_GTE: return ">=";
		case TOKEN_EQ: return "==";
		case TOKEN_NEQ: return "!=";
		case TOKEN_ASSIGN: return "=";
		case TOKEN_ASSIGN_ADD: return "+=";
		case TOKEN_ASSIGN_SUB: return "-=";
		case TOKEN_ASSIGN_MUL: return "*=";
		case TOKEN_ASSIGN_DIV: return "/=";
		case TOKEN_ASSIGN_MOD: return "%=";
		case TOKEN_ASSIGN_XOR: return "^=";
	}

	return NULL;
}

void token_pretty_print(enum TOKENS token, const char *default_)
{
	const char *symbols = token_get_symbols(token);
	if(symbols) {
		printf("<token:'%s'>\n", symbols);
	} else {
		printf("<token:%s>\n", default_);
	}
}

void token_dump_string(const char *s)
{
	printf("<string:\"");
	while(*s)
	{
		if(*s == '\r') {
			printf("\\r");
		} else if (*s == '\n') {
			printf("\\n");
		} else if (0x20 <= *s && *s <= 0x7e) {
			putchar(*s);
		} else {
			printf("\\x%.2x", (unsigned char)*s);
		}
		s++;
	}
	printf("\">\n");
}
