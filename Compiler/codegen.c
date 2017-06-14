#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "codegen.h"

void emit_global_vars(AST_NODE *prog)
{
	while(prog != NO_NODE) {
		if(prog->type == VAR_DECL) {
			AST_VAR *var = &prog->var;
			const char *vname = var->symbol->name;
			if(var->data_type != INT) {
				printf("/* ERROR: only int variable supported: %s */\n", vname);
				continue;
			}

			printf("field static int %s", vname);
			if(var->val != NO_NODE) {
				printf(" = %d", var->val->integer);
				if(var->var_type == CONST) {
					printf(" /* CONST VALUE */");
				}
			}
			putchar('\n');
		}
		prog = prog->next_stmt;
	}
}

void emit_local_vars(AST_NODE *prog)
{
	int i = 0;
	while(prog != NO_NODE) {
		if(prog->type == VAR_DECL) {
			AST_VAR *var = &prog->var;
			var->idx = i++;

			if(var->val == NO_NODE) {
				printf("/* var %s has no val */\n", var->symbol->name);
			} else {
				printf("/* var %s has val */\n", var->symbol->name);
				printf("sipush %d\n", var->val->integer);
				printf("istore_%d\n", var->idx);
			}
		}
		prog = prog->next_stmt;
	}
}

void emit_func_body(AST_FUNC *func)
{
	if(func->body == NO_NODE) {
		puts("/* func body is empty */");
		return;
	}
	emit_local_vars(func->body);

	// TODO
}

void emit_func_param(AST_FUNC *func)
{
	AST_NODE *params = func->params;
	int i = 0;

	while(params != NO_NODE) {
		if(i)
			printf(", ");
		params->var.idx = i++;
		if(params->var.data_type != INT) {
			printf("/* ERROR: only integer type are supported */ int");
		} else {
			printf("/* %s */ int", params->var.symbol->name);
		}

		params = params->next;
	}
}

void emit_funcs(AST_NODE *prog, int main)
{
	while(prog != NO_NODE) {
		if(prog->type == FUNC_DECL) {
			AST_FUNC *func = &prog->func;
			const char *fname = func->symbol->name;

			if(strcmp(fname, "main") == 0) {
				puts("method public static void main(java.lang.String[])");
				puts("max_stack 30");
				puts("max_locals 30");
				puts("{");
				emit_func_body(func);
				puts("return");
				puts("}\n");
			} else {
				const char *ftype = "void";
				if(func->return_type == INT) {
					ftype = "int";
				} else if (func->return_type != VOID) {
					printf("/* ERROR: only support int return type */");
				}
				printf("method public static %s %s(", ftype, fname);
				emit_func_param(func);
				printf(")\n");
				puts("max_stack 30");
				puts("max_locals 30");
				emit_func_body(func);
				puts("{");
				if(func->return_type == INT) {
					puts("sipush 0"); // RETURN 0
					puts("ireturn");
				} else {
					puts("return");
				}
				puts("}\n");
			}
		}
		prog = prog->next_stmt;
	}
}

void codegen(AST_NODE *prog)
{
	AST_NODE *p = prog;

	printf("class prog");
	puts("{");
	emit_global_vars(prog);
	putchar('\n');
	emit_funcs(prog, 1); // emit main func first
	puts("}");
}
