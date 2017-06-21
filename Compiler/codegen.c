#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "codegen.h"

#define cgerror(...) { fprintf(stderr, "CODEGEN_ERR:"); fprintf(stderr, __VA_ARGS__); fputc('\n', stderr); exit(1); }

int lblid = 0;

#define NEW_LABEL (10 * ++lblid)

const char *JAVA_STRING = "java.lang.String";

VARLIST *gvars = NULL;

const char *get_type(int typecode)
{
	switch(typecode) {
		case BOOL: return "boolean";
		case INT: return "int";
		case REAL: return "double";
		case STRING: return JAVA_STRING;
		case VOID: return "void";
		default: return "void /* ERROR: unsupported type */";
	}
}

const char *op_get(const AST_VAR *var)
{
	if(var->array_size > 0) {
		return "aload";
	}
	switch(var->data_type) {
		case BOOL:
		case INT: return "iload";
		case REAL: return "dload";
		case STRING: return "aload";
		default: return "iload /* ERROR: unsupported type */";
	}
}

const char *op_arr_get(int typecode)
{
	switch(typecode) {
		case BOOL:
		case INT: return "iaload";
		case REAL: return "daload";
		case STRING: return "aaload";
		default: return "iaload /* ERROR: unsupported type */";
	}
}

const char *op_set(const AST_VAR *var)
{
	if(var->array_size > 0) {
		return "astore";
	}
	switch(var->data_type) {
		case BOOL:
		case INT: return "istore";
		case REAL: return "dstore";
		case STRING: return "astore";
		default: return "iload /* ERROR: unsupported type */";
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

			VARLIST *v = malloc(sizeof(VARLIST));
			v->var = var;
			v->next = gvars;
			gvars = v;

			printf("field static %s%s %s", vtype, var->array_size > 0 ? "[]" : "", vname);
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

void scan_local_vars(const AST_NODE *prog, int *i)
{
	while(prog != NO_NODE) {
		if(prog->type == VAR_DECL) {
			AST_VAR *var = (AST_VAR *)&prog->var;
			printf("/* local var: %s, %d */\n", var->symbol->name, *i);
			var->idx = (*i)++;
			if(var->data_type == REAL) (*i)++;
		} else if(prog->type == FOR_STMT) {
			scan_local_vars(prog->for_stmt.init, i);
			scan_local_vars(prog->for_stmt.body, i);
		} else if(prog->type == IF_STMT) {
			scan_local_vars(prog->if_stmt.true_stmt, i);
			scan_local_vars(prog->if_stmt.false_stmt, i);
		}
		prog = prog->next_stmt;
	}
}

int emit_load_var(const AST_VAR *var)
{
	const char *vname = var->symbol->name;
	printf("/* load var %s (%d) */\n", vname, var->idx);

	if(var->idx == -1) { // global variable / constants
		printf("getstatic %s%s prog.%s\n", get_type(var->data_type), var->array_size > 0 ? "[]" : "", vname);
	} else { // function arguments or local variable
		printf("%s %d\n", op_get(var), var->idx);
	}

	return var->data_type;
}

void emit_atomic_store(const AST_VAR *var)
{
	const char *vname = var->symbol->name;
	printf("/* store var %s (%d) */\n", vname, var->idx);

	if(var->idx == -1) { // global
		printf("putstatic %s%s prog.%s\n", get_type(var->data_type), var->array_size > 0 ? "[]" : "", vname);
	} else { // local
		printf("%s %d\n", op_set(var), var->idx);
	}
}

int emit_val(const AST_VALUE *val)
{
	if(val->data_type == INT) {
		printf("sipush %d\n", val->integer);
	} else if (val->data_type == BOOL) {
		printf("iconst_%d\n", val->integer ? 1 : 0);
	} else if (val->data_type == STRING) {
		printf("ldc "); ast_dump_str(val->string); putchar('\n');
	} else if (val->data_type == REAL) {
		printf("ldc2_w %lf\n", val->real);
	} else {
		printf("/* ERROR: unsupported value type */\n");
		cgerror("unsupported value type");
	}
	return val->data_type;
}

void emit_convert(int from, int to)
{
	if(from == to) {
		return;
	}

	switch(from) {
		case BOOL:
		case INT:
			if(to == BOOL || to == INT) {
				return;
			}
			puts("i2d");
			break;
		case REAL:
			puts("d2i");
			break;
		case STRING:
			cgerror("can not covnert from string");
			break;
		case VOID:
			cgerror("convert from error");
			break;
	}
}

int emit_unary_expr(const AST_NODE *node)
{
	int label;
	const AST_EXPR *expr = &node->expr;
	int dtype = emit_expr(expr->lval);

	switch(expr->op) {
		case NEGATIVE:
			puts("ineg");
			break;
		case LOGICAL_NOT:
			label = NEW_LABEL;
			puts("/* begin logical not */");
			emit_label("ifeq", label, "1");
			puts("iconst_0");
			emit_label("goto", label, "end");
			emit_label_def(label, "1");
			puts("iconst_1");
			emit_label_def(label, "end");
			puts("/* end logical not */");
			break;
		case BITWISE_NOT:
			puts("iconst_m1");
			puts("ixor");
			break;
		case STRVAL:
			printf("invokestatic %s java.lang.String.valueOf(%s)\n", JAVA_STRING, get_type(dtype));
			break;
		default:
			printf("sipush 0 /* ERROR: unsupported operator: %d */\n", expr->op);
	}

	return expr->data_type;
}

void emit_binary_cond(const char *opcode, int typecode)
{
	int label = NEW_LABEL;
	printf("/* begin binary cond, typecode = %d */\n", typecode);
	if(typecode == INT || typecode == BOOL) {
		puts("isub");
	} else if (typecode == REAL) {
		puts("dcmpl");
	} else {
		cgerror("unsupported data type in emit_binary_cond");
	}
	emit_label(opcode, label, "cond_true");
	puts("iconst_0");
	emit_label("goto", label, "cond_end");
	emit_label_def(label, "cond_true");
	puts("iconst_1");
	emit_label_def(label, "cond_end");
	puts("/* end binary cond */");
}

const char *numerical_op(int op, int typecode)
{
	if(typecode == INT) {
		switch(op) {
			case ADD: return "iadd";
			case SUB: return "isub";
			case DIV: return "idiv";
			case MOD: return "irem";
			case MUL: return "imul";
		}
		cgerror("unsupported op");
		goto err;
	} else if(typecode == REAL) {
		switch(op) {
			case ADD: return "dadd";
			case SUB: return "dsub";
			case DIV: return "ddiv";
			case MOD: return "drem";
			case MUL: return "dmul";
		}
		cgerror("unsupported op");
		goto err;
	}
	cgerror("unsupported type");
err:
	return "iadd /* unsupported op */";
}

int emit_binary_expr(const AST_NODE *node)
{
	const AST_EXPR *expr = &node->expr;
	int t;

	t = emit_expr(expr->lval);
	if(expr->data_type == REAL && expr->op != ARRGET) emit_convert(t, REAL);
	t = emit_expr(expr->rval);
	if(expr->data_type == REAL && expr->op != ARRGET) emit_convert(t, REAL);

	switch(expr->op) {
		// string only
		case STREQU: printf("invokevirtual boolean %s.equals(java.lang.Object)\n", JAVA_STRING); break;
		case STRCAT: printf("invokevirtual %1$s %1$s.concat(%1$s)\n", JAVA_STRING); break;

		// numerical operation
		case ADD:
		case SUB:
		case DIV:
		case MOD:
		case MUL: puts(numerical_op(expr->op, expr->data_type)); break;

		case EQ:  emit_binary_cond("ifeq", expr->data_type); break;
		case NEQ: emit_binary_cond("ifne", expr->data_type); break;
		case LT:  emit_binary_cond("iflt", expr->data_type); break;
		case GT:  emit_binary_cond("ifgt", expr->data_type); break;
		case LTE: emit_binary_cond("ifle", expr->data_type); break;
		case GTE: emit_binary_cond("ifge", expr->data_type); break;

		// int only
		case XOR: puts("ixor"); break;
		case BITWISE_AND:
		case LOGICAL_AND: puts("iand"); break;
		case BITWISE_OR:
		case LOGICAL_OR:  puts("ior"); break;

		// special
		case ARRGET:      puts(op_arr_get(expr->data_type)); break;
		default: printf("iadd /* ERROR: unsupported operator: %d */\n", expr->op); break;
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
			cgerror("invalid expr type");
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
	emit_func_param(func, 0);
	printf(")\n");

	return func->return_type;
}

void emit_assign(const AST_ASSIGN *assign)
{
	if(assign->index == NO_NODE) {
		int t = emit_expr(assign->rval);
		emit_convert(t, assign->lval->data_type);
		emit_atomic_store(assign->lval);
	} else {
		// assign to array
		if(ast_get_expr_type(assign->index) != INT) {
			cgerror("array index must be INT");
		}

		emit_load_var(assign->lval);
		emit_expr(assign->index);
		int t = emit_expr(assign->rval);
		emit_convert(t, assign->lval->data_type);
		switch(assign->lval->data_type) {
			case BOOL:
			case INT: puts("iastore"); break;
			case REAL: puts("dastore"); break;
			case STRING: puts("aastore"); break;
		}
	}
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
	int label = NEW_LABEL;
	int has_else_part = ifs->false_stmt != NO_NODE;

	puts("/* begin if condition */");
	emit_expr(ifs->cond);
	puts("/* end of if condition */");

	emit_label("ifeq", label, "else");

	puts("/* begin true statements */");
	emit_stmts(ifs->true_stmt);
	puts("/* end of true statements */");
	if(has_else_part) {
		emit_label("goto", label, "end");
	}

	emit_label_def(label, "else");

	if(has_else_part) {
		puts("/* begin false statements */");
		emit_stmts(ifs->false_stmt);
		puts("/* end of false statements */");
		emit_label_def(label, "end");
	} else {
		puts("/* if statement without ELSE part */");
	}

	puts("nop");
}

void emit_for(const AST_FOR *fors)
{
	int label = NEW_LABEL;

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

void emit_read(const AST_NODE *expr)
{
	if(expr->child->type != VAR_DECL) {
		printf("/* ERROR: expected a char, got %s */\n", ast_get_node_type(expr->child->type));
		exit(1);
	}

	const AST_VAR *var = &expr->child->var;

	switch(var->data_type) {
		case BOOL:
		case INT:
			puts("invokestatic int Reader.readInteger()");
			break;
		case REAL:
			puts("invokestatic double Reader.readDouble()");
			break;
		case STRING:
			printf("invokestatic %s Reader.readLine()\n", JAVA_STRING);
			break;
	}

	emit_atomic_store(var);
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

void emit_var(const AST_VAR *var)
{
	if(var->val != NO_NODE) {
		// non-array
		printf("/* var#%-2d %s has val */\n", var->idx, var->symbol->name);
		emit_val(var->val);
		emit_atomic_store(var);
	} else if(var->array_size > 0) {
		// array
		printf("/* var#%-2d %s is array */\n", var->idx, var->symbol->name);
		printf("sipush %d\n", var->array_size);
		if(var->data_type == STRING) putchar('a'); // anewarray
		printf("newarray %s\n", get_type(var->data_type));
		emit_atomic_store(var);
	} else {
		printf("/* var#%-2d %s has no val */\n", var->idx, var->symbol->name);
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
				emit_read(stmt);
				break;
			case VAR_DECL:
				emit_var(&stmt->var);
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
	scan_local_vars(func->body, &pcount);
	emit_stmts(func->body);
}

void emit_func_param(const AST_FUNC *func, int set_idx)
{
	AST_NODE *params = (AST_NODE *)func->params;
	int i = 0;

	while(params != NO_NODE) {
		if(i) printf(", ");
		if(set_idx) params->var.idx = i;
		i++;
		if(params->var.data_type == REAL) i++;

		printf("/* %s */ %s", params->var.symbol->name, get_type(params->var.data_type));

		params = params->next;
	}
}

void emit_funcs(AST_NODE *prog)
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
				puts("/* init global array */");
				VARLIST *v = gvars;
				while(v) {
					emit_var(v->var);
					v = v->next;
				}
				puts("/* begin program */");
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
				emit_func_param(func, 1);
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
	emit_funcs(prog);
	puts("}");
}
