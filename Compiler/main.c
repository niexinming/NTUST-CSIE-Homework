#include <stdio.h>

#include "codegen.h"
#include "ast.h"

extern int parser_trace;
extern int scanner_dumpline;
extern SYMTAB *symtab, *root_symtab;
extern AST_NODE *prog;

void usage(char *argv[]) {
	printf("%s <opts> <filename>\n"
	       "\n"
	       "  opts:\n"
	       "  -d    dump ast\n"
	       "  -t    enable parser trace\n"
	       "  -l    dump line\n",
	       argv[0]);
}

int main(int argc, char *argv[])
{
	symtab = root_symtab = symtab_create(NULL);

	int opts[256] = {0};
	const char *filename = NULL;

	for(int i = 1; i < argc; i++) {
		const char *opt = argv[i];
		if(opt[0] == '-') {
			const char *o = opt + 1;
			while(*o) {
				opts[(unsigned char)*(o++)]++;
			}
		} else {
			filename = argv[i];
		}
	}

	if(opts['h']) {
		usage(argv);
		return 0;
	}

	if(opts['t']) {
		parser_trace = 1;
	}

	if(opts['l']) {
		scanner_dumpline = 1;
	}

    if(filename) {
        freopen(filename, "r", stdin);
    }

	yyparse();

	if(opts['d']) {
		ast_dump_stmt_body(prog);
	} else if(!scanner_dumpline && !parser_trace) {
		codegen(prog);
	} else {
		fprintf(stderr, "NOTICE: disable codegen due to dumpline or trace enabled\n");
	}
}
