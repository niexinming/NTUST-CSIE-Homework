#include <stdio.h>
#include "tokens.h"

const char * token_get_symbols(enum yytokentype token)
{
	switch(token) {
		case COMMA: return ",";
		case COLON: return ":";
		case SEMICOLON: return ";";
		case LEFT_PARENTHESIS: return "(";
		case RIGHT_PARENTHESIS: return ")";
		case LEFT_BRACKET: return "[";
		case RIGHT_BRACKET: return "]";
		case LEFT_BRACE: return "{";
		case RIGHT_BRACE: return "}";
		case LOGICAL_NOT: return "!";
		case BITWISE_NOT: return "~";
		case ADD: return "+";
		case SUB: return "-";
		case MUL: return "*";
		case DIV: return "/";
		case MOD: return "%";
		case XOR: return "^";
		case BITWISE_AND: return "&";
		case BITWISE_OR: return "|";
		case LOGICAL_AND: return "&&";
		case LOGICAL_OR: return "||";
		case LT: return "<";
		case GT: return ">";
		case LTE: return "<=";
		case GTE: return ">=";
		case EQ: return "==";
		case NEQ: return "!=";
		case ASSIGN: return "=";
		case ASSIGN_ADD: return "+=";
		case ASSIGN_SUB: return "-=";
		case ASSIGN_MUL: return "*=";
		case ASSIGN_DIV: return "/=";
		case ASSIGN_MOD: return "%=";
		case ASSIGN_XOR: return "^=";
		default: return NULL;
	}
}

void token_pretty_print(enum yytokentype token, const char *default_)
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
