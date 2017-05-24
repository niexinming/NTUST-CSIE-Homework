#include "parser.h"

const char * token_get_symbols(enum yytokentype);
void token_pretty_print(enum yytokentype, const char *);
void token_dump_string(const char *);
