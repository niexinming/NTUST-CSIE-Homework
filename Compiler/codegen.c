#include <stdlib.h>
#include <stdio.h>
#include "codegen.h"

void codegen(AST_NODE *prog)
{
	puts("codegen");
	while(prog && prog != NO_NODE) {
		ast_dump_stmt(prog);
		prog = prog->next_stmt;
	}
}
