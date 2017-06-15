#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "codegen.h"

const char *JAVA_STRING = "java.lang.String";

const char *get_type(int typecode)
{
	switch(typecode) {
		case INT: return "int";
		case STRING: return JAVA_STRING;
		case VOID: return "void";
		default: return "void /* ERROR: unsupported type */";
	}
}

void emit_global_vars(AST_NODE *prog)
{
	while(prog != NO_NODE) {
		if(prog->type == VAR_DECL) {
			AST_VAR *var = &prog->var;
			var->idx = -1;
			const char *vname = var->symbol->name;
			const char *vtype = get_type(var->data_type);

			printf("field static %s %s", vtype, vname);
			if(var->val != NO_NODE) {
				if(var->data_type == INT) {
					printf(" = %d", var->val->integer);
				} else if(var->data_type == STRING) {
					printf(" = ");
					ast_dump_str(var->val->string);
				}
				if(var->var_type == CONST) {
					printf(" /* CONST VALUE */");
				}
			}
			putchar('\n');
		}
		prog = prog->next_stmt;
	}
}

void emit_local_vars(const AST_NODE *prog, int i)
{
	while(prog != NO_NODE) {
		if(prog->type == VAR_DECL) {
			AST_VAR *var = (AST_VAR *)&prog->var;
			var->idx = i++;

			if(var->val == NO_NODE) {
				printf("/* var#%-2d %s has no val */\n", var->idx, var->symbol->name);
			} else {
				printf("/* var#%-2d %s has val */\n", var->idx, var->symbol->name);
				emit_val(var->val);
				emit_store(var);
			}
		}
		prog = prog->next_stmt;
	}
}

void emit_var(const AST_VAR *var)
{
	const char *vname = var->symbol->name;

	if(var->idx == -1) {
		// global variable / constants
		const char *vtype = get_type(var->data_type);
		printf("getstatic %s prog.%s\n", vtype, vname);
	} else {
		// function arguments or local variable
		if(var->data_type == INT) {
			printf("iload %d /* %s */\n", var->idx, vname);
		} else if(var->data_type == STRING) {
			printf("aload %d /* %s */\n", var->idx, vname);
		} else {
			printf("/* ERROR: var loading unsupported type: %s */\n", ast_get_type_name(var->data_type));
		}
	}
}

void emit_store(const AST_VAR *var)
{
	if(var->idx == -1) {
		// global
		const char *vtype;
		if(var->data_type == INT) {
			vtype = "int";
		} else if(var->data_type == STRING) {
			vtype = "java.io.String";
		} else {
			printf("/* ERROR: unsupported store data type */\n");
		}

		printf("putstatic %s prog.%s\n", vtype, var->symbol->name);
	} else {
		// local
		if(var->data_type == INT) {
			printf("istore %d\n", var->idx);
		} else if(var->data_type == STRING) {
			printf("astore %d\n", var->idx);
		} else {
			printf("/* ERROR: unsupported store data type */\n");
		}
	}
}

void emit_val(const AST_VALUE *val)
{
	if(val->data_type == INT) {
		printf("sipush %d\n", val->integer);
	} else if (val->data_type == STRING) {
		printf("ldc "); ast_dump_str(val->string); putchar('\n');
	} else {
		printf("/* ERROR: unsupported value type */\n");
	}
}

int emit_expr(const AST_NODE *expr)
{
	switch(expr->type) {
		case CONST_VAL:
			emit_val(&expr->val);
			return expr->val.data_type;
		case VAR_DECL:
		case FUNC_CALL:
		case EXPR_UNARY:
		case EXPR_BINARY:
			printf("sipush 0 /* expr */\n");
			return INT;
		default:
			printf("/* ERROR: invalid expr type */\n");
			return VOID;
	}
}

void emit_invoke(const AST_INVOKE *invoke)
{
	const AST_FUNC *func = invoke->func;
	const AST_NODE *arg = invoke->args;

	while(arg != NO_NODE) {
		emit_expr(arg);
		arg = arg->next;
	}

	const char *fname = func->symbol->name;
	const char *ftype = get_type(func->return_type);
	printf("invokestatic %s prog.%s(", ftype, fname);
	emit_func_param(func);
	printf(")\n");
}

void emit_assign(const AST_ASSIGN *assign)
{
	emit_expr(assign->rval);
	emit_store(assign->lval);
}

void emit_eval(const AST_NODE *node)
{

}

void emit_if(const AST_NODE *ifs)
{

}

void emit_print(const AST_NODE *expr, int flag)
{
	const char *fname = flag == NEWLINE ? "println" : "print";

	puts("getstatic java.io.PrintStream java.lang.System.out");
	int type = emit_expr(expr);
	printf("invokevirtual void java.io.PrintStream.%s(%s)\n", fname, get_type(type));
}

void emit_func_body(AST_FUNC *func)
{
	if(func->body == NO_NODE) {
		puts("/* func body is empty */");
		return;
	}

	int pcount = func->param_count;
	if(strcmp(func->symbol->name, "main") == 0) pcount = 1;
	emit_local_vars(func->body, pcount);

	const AST_NODE *stmt = func->body;

	while(stmt != NO_NODE) {
		switch(stmt->type) {
			case FUNC_CALL:
				emit_invoke(&stmt->invoke);
				break;
			case ASSIGN_STMT:
				emit_assign(&stmt->assignment);
				break;
			case PRINT_STMT:
				emit_print(stmt->child, stmt->flag);
				break;
			case RETURN_STMT:
			case IF_STMT:
			case FOR_STMT:
				printf("/* STMT: %s */\n", ast_get_node_type(stmt->type));
				break;

			case READ_STMT:
				printf("/* Not implemented stmt: READ */\n");
				break;

			case VAR_DECL:
				break;

			case CONST_VAL:
			case FUNC_DECL:
			case EXPR_UNARY:
			case EXPR_BINARY:
				printf("/* Invalid stmt here: %s */\n", ast_get_node_type(stmt->type));
		}
		stmt = stmt->next_stmt;
	}
}

void emit_func_param(AST_FUNC *func)
{
	AST_NODE *params = (AST_NODE *)func->params;
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
				puts("{");
				emit_func_body(func);
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
