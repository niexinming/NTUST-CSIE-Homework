#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "codegen.h"

int lblid = 0;

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

void emit_local_vars(const AST_NODE *prog, int *i)
{
	while(prog != NO_NODE) {
		if(prog->type == VAR_DECL) {
			AST_VAR *var = (AST_VAR *)&prog->var;
			var->idx = (*i)++;

			if(var->val == NO_NODE) {
				printf("/* var#%-2d %s has no val */\n", var->idx, var->symbol->name);
			} else {
				printf("/* var#%-2d %s has val */\n", var->idx, var->symbol->name);
				emit_val(var->val);
				emit_store(var);
			}
		} else if(prog->type == FOR_STMT) {
			emit_local_vars(prog->for_stmt.init, i);
			emit_local_vars(prog->for_stmt.body, i);
		} else if(prog->type == IF_STMT) {
			emit_local_vars(prog->if_stmt.true_stmt, i);
			emit_local_vars(prog->if_stmt.false_stmt, i);
		}
		prog = prog->next_stmt;
	}
}

int emit_load_var(const AST_VAR *var)
{
	const char *vname = var->symbol->name;

	printf("/* load var %s (%d) */\n", vname, var->idx);

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

	return var->data_type;
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

int emit_val(const AST_VALUE *val)
{
	if(val->data_type == INT) {
		printf("sipush %d\n", val->integer);
	} else if (val->data_type == STRING) {
		printf("ldc "); ast_dump_str(val->string); putchar('\n');
	} else {
		printf("/* ERROR: unsupported value type */\n");
	}
	return val->data_type;
}

int emit_unary_expr(const AST_NODE *node)
{
	const AST_EXPR *expr = &node->expr;
	switch(expr->op) {
		case NEGATIVE:
			emit_expr(expr->lval);
			puts("ineg");
			break;
		default:
			puts("sipush 0 /* ERROR: unsupported operator */");
	}

	return expr->data_type;
}

void emit_binary_cond(const char *opcode)
{
	int label = 10 * ++lblid;
	puts("isub");
	emit_label(opcode, label, "cond_true");
	puts("iconst_0");
	emit_label("goto", label, "cond_end");
	emit_label_def(label, "cond_true");
	puts("iconst_1");
	emit_label_def(label, "cond_end");
}

int emit_binary_expr(const AST_NODE *node)
{
	const AST_EXPR *expr = &node->expr;
	emit_expr(expr->rval);
	emit_expr(expr->lval);
	switch(expr->op) {
		case ADD: puts("iadd"); break;
		case SUB: puts("isub"); break;
		case DIV: puts("idiv"); break;
		case MUL: puts("imul"); break;
		case EQ:  emit_binary_cond("ifeq"); break;
		case NEQ: emit_binary_cond("ifne"); break;
		case LT:  emit_binary_cond("iflt"); break;
		case GT:  emit_binary_cond("ifgt"); break;
		case LTE: emit_binary_cond("ifle"); break;
		case GTE: emit_binary_cond("ifge"); break;
		default: puts("iadd /* ERROR: unsupported operator */"); break;
	}
	return expr->data_type;
}

int emit_expr(const AST_NODE *expr)
{
	switch(expr->type) {
		case CONST_VAL:
			return emit_val(&expr->val);
		case VAR_DECL:
			return emit_load_var(&expr->var);
		case FUNC_CALL:
			return emit_invoke(&expr->invoke);
		case EXPR_UNARY:
			return emit_unary_expr(expr);
		case EXPR_BINARY:
			return emit_binary_expr(expr);
		default:
			printf("/* ERROR: invalid expr type */\n");
			return VOID;
	}
}

int emit_invoke(const AST_INVOKE *invoke)
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

	return func->return_type;
}

void emit_assign(const AST_ASSIGN *assign)
{
	emit_expr(assign->rval);
	emit_store(assign->lval);
}

void emit_label(const char * prefix, int lblid, const char *subid)
{
	if(prefix) printf("%s ", prefix);
	printf("L_%d_%s\n", lblid, subid);
}

void emit_label_def(int lblid, const char *subid)
{
	printf("L_%d_%s:\n", lblid, subid);
}

void emit_if(const AST_IF *ifs)
{
	int label = 10 * ++lblid;

	puts("/* begin if condition */");
	emit_expr(ifs->cond);
	puts("/* end of if condition */");

	emit_label("ifeq", label, "else");

	puts("/* begin true statements */");
	emit_stmts(ifs->true_stmt);
	puts("/* end of true statements */");

	emit_label_def(label, "else");

	puts("/* begin false statements */");
	emit_stmts(ifs->false_stmt);
	puts("/* end of false statements */");
}

void emit_for(const AST_FOR *fors)
{
	int label = 10 * ++lblid;

	puts("/* for init */");
	emit_stmts(fors->init);

	emit_label_def(label, "beginfor");
	puts("/* for condition */");
	emit_expr(fors->cond);
	emit_label("ifeq", label, "endfor");
	puts("");

	puts("/* for body */");
	emit_stmts(fors->body);

	puts("/* for increment */");
	emit_stmts(fors->increment);
	emit_label("goto", label, "beginfor");

	emit_label_def(label, "endfor");
}

void emit_print(const AST_NODE *expr, int flag)
{
	const char *fname = flag == NEWLINE ? "println" : "print";

	puts("getstatic java.io.PrintStream java.lang.System.out");
	int type = emit_expr(expr);
	printf("invokevirtual void java.io.PrintStream.%s(%s)\n", fname, get_type(type));
}

void emit_return(const AST_NODE *expr)
{
	if(expr == NO_NODE) {
		puts("return");
		return;
	}
	int type = emit_expr(expr);
	switch(type) {
		case INT: puts("ireturn"); break;
		case STRING: puts("areturn"); break;
		default: puts("return /* ERROR: unknown return type */"); break;
	}
}

void emit_stmts(const AST_NODE *stmt)
{
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
				emit_return(stmt->child);
				break;
			case IF_STMT:
				emit_if(&stmt->if_stmt);
				break;
			case FOR_STMT:
				emit_for(&stmt->for_stmt);
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
				printf("/* Not implemented STMT: %s */\n", ast_get_node_type(stmt->type));
				printf("/* Invalid stmt here: %s */\n", ast_get_node_type(stmt->type));
		}
		stmt = stmt->next_stmt;
	}
}

void emit_func_body(AST_FUNC *func)
{
	if(func->body == NO_NODE) {
		puts("/* func body is empty */");
		return;
	}

	int pcount = func->param_count;
	if(strcmp(func->symbol->name, "main") == 0) pcount = 1;
	emit_local_vars(func->body, &pcount);
	emit_stmts(func->body);
}

void emit_func_param(const AST_FUNC *func)
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
					puts("/* function end */");
					puts("sipush 0"); // RETURN 0
					puts("ireturn");
				} else {
					puts("/* function end */");
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