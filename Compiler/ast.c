#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "ast.h"

#define AST_INDENT_SIZE 2
#define CASE_STR(X) case X: return #X;

int ast_indent_size = 0;

void yyerror(const char *errmsg);

void ast_indent()
{
	ast_indent_size += AST_INDENT_SIZE;
}

void ast_unindent()
{
	ast_indent_size -= AST_INDENT_SIZE;
}

void ast_pindent()
{
	int i;
	for(i = 0; i < ast_indent_size; i++) {
		putchar(' ');
	}
}

const struct AST_NODE_s NODE_CONST_STR_BR = {
	NULL, // next
	NULL, // next_stmt
	CONST_VAL, // type
	0, // flag
	{
		.val = {
			STRING,
			{
				.string = "\n"
			}
		}
	}
};

const char * ast_get_type_name(int typecode)
{
	switch(typecode) {
		case VOID:   return "VOID";
		case INT:    return "INT";
		case BOOL:   return "BOOL";
		case REAL:   return "REAL";
		case STRING: return "STRING";
		default:     return "UNKNOWN";
	}
}

const char * ast_get_var_type(int var_type)
{
	switch (var_type) {
		case CONST: return "CONST";
		case VAR:   return "VAR";
		case PARAM: return "PARAM";
		default:    return "UNKNOWN";
	}
}

const char * ast_get_op_str(int op)
{
	switch(op) {
		case LOGICAL_OR:  return "||";
		case LOGICAL_AND: return "&&";
		case LT:          return "<";
		case GT:          return ">";
		case LTE:         return "<=";
		case GTE:         return ">=";
		case EQ:          return "==";
		case NEQ:         return "!=";
		case BITWISE_OR:  return "|";
		case BITWISE_AND: return "&";
		case ADD:         return "+";
		case SUB:         return "-";
		case XOR:         return "^";
		case MUL:         return "*";
		case DIV:         return "/";
		case MOD:         return "%";
		case LOGICAL_NOT: return "!";
		case BITWISE_NOT: return "~";
		case NEGATIVE:    return "-";
		default:          assert(NULL); // unknown op
	}
	return NULL;
}

const char * ast_get_node_type(enum AST_TYPE t)
{
	switch (t) {
		CASE_STR(CONST_VAL)
		CASE_STR(VAR_DECL)
		CASE_STR(FUNC_DECL)
		CASE_STR(FUNC_CALL)
		CASE_STR(ASSIGN_STMT)
		CASE_STR(PRINT_STMT)
		CASE_STR(READ_STMT)
		CASE_STR(RETURN_STMT)
		CASE_STR(IF_STMT)
		CASE_STR(FOR_STMT)
		CASE_STR(EXPR_UNARY)
		CASE_STR(EXPR_BINARY)
	}
	return NULL;
}

AST_NODE* ast_create_node(enum AST_TYPE type)
{
	AST_NODE *ast_node = malloc(sizeof(AST_NODE));
	ast_node->next = NULL;
	ast_node->type = type;
	ast_node->child = NO_NODE;
	return ast_node;
}

unsigned int ast_node_length(AST_NODE *node)
{
	assert(node);
	unsigned int d = 0;
	while(node != NO_NODE) {
		d++;
		node = node->next;
	}
	return d;
}

AST_NODE* ast_create_value_node(int type)
{
	AST_NODE *node = ast_create_node(CONST_VAL);
	node->val.data_type = type;
    return node;
}

void ast_dump_var(const AST_VAR* var)
{
	assert(var);

	if(var == NO_NODE) {
		puts("<VAR(NO_NODE)>");
		return;
	}

    const char *var_type = ast_get_var_type(var->var_type);
    const char *data_type = ast_get_type_name(var->data_type);
    printf("<%s(Name=%s, Type=%s", var_type, var->symbol->name, data_type);
    if(var->val != NO_NODE) {
        printf(", DefaultValue=");
        switch (var->data_type) {
            case VOID: printf("VOID"); break;
            case INT: printf("%d", var->val->integer); break;
            case REAL: printf("%lg", var->val->real); break;
            case STRING: printf("\"%s\"", var->val->string); break;
            case BOOL: printf("%s", var->val->integer ? "TRUE" : "FALSE");
        }
    }
    if(var->array_size) {
        printf(", ArraySize=%d", var->array_size);
    }
    printf(")>");
}

AST_NODE* ast_create_var_node(int data_type, int var_type,
		unsigned int array_size, SYMTAB_ENTRY *symbol,
		struct AST_VALUE_s * default_val)
{
	assert(symbol && default_val);
	if(default_val != NO_NODE && data_type != default_val->data_type) {
		yyerror("Data type mismatch for var/const declaration");
		return NULL;
	}

	AST_NODE *node = ast_create_node(VAR_DECL);
    AST_VAR *var = &node->var;

    var->data_type = data_type;
    var->var_type = var_type;
    var->array_size = array_size;
    var->symbol = symbol;
    var->val = default_val;

    return node;
}

AST_NODE* ast_create_func_node(SYMTAB_ENTRY *symbol, int return_type,
		AST_NODE *params, AST_NODE *body)
{
	AST_NODE *node = ast_create_node(FUNC_DECL);
	AST_FUNC *func = &node->func;

	if(!params) {
		params = NO_NODE;
	}

	func->symbol = symbol;
	func->return_type = return_type;
	func->params = params;
	func->param_count = ast_node_length(params);
	func->body = body;

	return node;
}

AST_NODE* ast_create_invoke_node(AST_FUNC *func, AST_NODE *args)
{
	AST_NODE *node = ast_create_node(FUNC_CALL);
	AST_INVOKE *invoke = &node->invoke;

	invoke->func = func;
	invoke->args = args;
	invoke->args_count = ast_node_length(args);

	return node;
}

int ast_is_non_void_type(int t)
{
	switch(t) {
		case INT:
		case BOOL:
		case STRING:
		case REAL:
			return 1;
	}
	return 0;
}

void ast_dump_str(const char *str)
{
	putchar('"');
	while(*str) {
		switch(*str) {
			case '"':
				printf("\\\"");
				break;
			case '\n':
				printf("\\n");
				break;
			default:
				putchar(*str);
		}
		str++;
	}
	putchar('"');
}

void ast_dump_const_val(const AST_VALUE *val)
{
	assert(val);
	switch(val->data_type) {
		case INT:
			printf("<CONST(Type=INT, VAL=%d)>", val->integer);
			break;
		case BOOL:
			printf("<CONST(Type=BOOL, VAL=%s)>", val->integer ? "TRUE" : "FALSE");
			break;
		case STRING:
			printf("<CONST(Type=STRING, VAL=");
			ast_dump_str(val->string);
			printf(")>");
			break;
		case REAL:
			printf("<CONST(Type=REAL, VAL=%lg)>", val->real);
			break;
		default:
			printf("<CONST(Type=?)>");
	}
}

void ast_dump_func_call(const AST_INVOKE *invoke)
{
	assert(invoke);
	printf("<CALL(FUNC=%s, ARGS=[", invoke->func->symbol->name);
	const AST_NODE *arg = invoke->args;
	for(int i = invoke->args_count - 1; i >= 0; i--) {
		if(arg == NULL) { yyerror("missing arg"); assert(NULL); }

		ast_dump_expr(arg);
		printf("%s", i ? ", " : "])>");

		arg = arg->next;
	}
}

void ast_dump_expr(const AST_NODE *node)
{
	assert(node && node != NO_NODE);
	switch(node->type) {
		case CONST_VAL:
			ast_dump_const_val(&node->val);
			break;
		case VAR_DECL:
			printf("<REF(Name=%s)>", node->var.symbol->name);
			break;
		case FUNC_CALL:
			ast_dump_func_call(&node->invoke);
			break;
		case EXPR_UNARY:
			printf("( %s ", ast_get_op_str(node->expr.op));
			ast_dump_expr(node->expr.lval);
			printf(" )");
			break;
		case EXPR_BINARY:
			printf("( ");
			ast_dump_expr(node->expr.lval);
			printf(" %s ", ast_get_op_str(node->expr.op));
			ast_dump_expr(node->expr.rval);
			printf(" )");
			break;
		default:
			assert(NULL); // not a expr node
	}
}

void ast_dump_stmt_body(const AST_NODE *p)
{
	assert(p);
	while(p && p != NO_NODE) {
		ast_pindent();
		ast_dump_stmt(p);
		putchar('\n');
		p = p->next_stmt;
	}
}

void ast_dump_func(const AST_NODE *stmt)
{
	assert(stmt && stmt->type == FUNC_DECL);

	const AST_FUNC *func = &stmt->func;
	const AST_NODE *param = func->params;

	printf("<FUNC(Name=%s", func->symbol->name);
	if(func->param_count) {
		printf(", Params=[\n");
		for(int i = func->param_count - 1; i >= 0; i--) {
			assert(param);
			ast_pindent();
			ast_dump_var(&param->var);
			printf("%s", i ? ",\n" : "\n]");
			param = param->next;
		}
	} else {
		printf(", NoParams");
	}

	printf(", Body={\n");
	ast_indent();
	ast_dump_stmt_body(func->body);
	ast_unindent();
	printf("}>\n");
}

void ast_dump_assign(const AST_ASSIGN *assign)
{
	assert(assign);
	printf("<ASSIGN(TARGET=%s, VAL=", assign->lval->symbol->name);
	ast_dump_expr(assign->rval);
	printf(">");
}

void ast_dump_if_stmt(const AST_NODE *stmt)
{
	assert(stmt);
	assert(stmt->type == IF_STMT);

	printf("<IF(Cond=");
	ast_dump_expr(stmt->if_stmt.cond);
	printf(", TRUE={\n");

	ast_indent();
	ast_dump_stmt_body(stmt->if_stmt.true_stmt);
	ast_unindent();

	const AST_NODE *F = stmt->if_stmt.false_stmt;
	if(F && F != NO_NODE) {
		ast_pindent();
		printf("}, FALSE={\n");
		ast_indent();
		ast_dump_stmt_body(F);
		ast_unindent();
	}

	ast_pindent();
	printf("}>");
}

void ast_dump_for_stmt(const AST_NODE *stmt)
{
	assert(stmt);
	const AST_FOR *f = &stmt->for_stmt;

	printf("<FOR(Init=");
	ast_dump_stmt(f->init);
	printf(", Cond=");
	ast_dump_expr(f->cond);
	printf(", Increment=");
	ast_dump_stmt(f->increment);
	printf(", Body={\n");
	ast_indent();
	ast_dump_stmt_body(f->body);
	ast_unindent();
	ast_pindent();
	printf("}>");
}

void ast_dump_stmt(const AST_NODE *stmt)
{
	assert(stmt);
	if(stmt == NO_NODE) {
		printf("<NONE>");
		return;
	}
	switch(stmt->type) {
		case VAR_DECL:
			ast_dump_var(&stmt->var);
			break;
		case FUNC_DECL:
			ast_dump_func(stmt);
			break;
		case ASSIGN_STMT:
			ast_dump_assign(&stmt->assignment);
			break;
		case PRINT_STMT:
			printf("<PRINT(Val=");
			ast_dump_expr(stmt->child);
			printf(")>");
			break;
		case READ_STMT:
			printf("<READ(Var=");
			ast_dump_var(&stmt->child->var);
			printf(")>");
			break;
		case RETURN_STMT:
			printf("<RETURN(Val=");
			ast_dump_expr(stmt->child);
			printf(")>");
			break;
		case IF_STMT:
			ast_dump_if_stmt(stmt);
			break;
		case FOR_STMT:
			ast_dump_for_stmt(stmt);
			break;
		case FUNC_CALL:
			ast_dump_expr(stmt);
			break;
		case CONST_VAL:
		case EXPR_UNARY:
		case EXPR_BINARY:
			puts("Invalid statment type");
			exit(0);
			break;
	}
}

AST_NODE* ast_create_expr_node(const AST_NODE *l, int op, const AST_NODE *r)
{
	assert(l != NULL && r != NULL); int ltype = ast_get_expr_type(l);
	int rtype = ast_get_expr_type(r);

	// printf("NODE_TYPE: %d %d\n", l->type, r == NO_NODE ? -1 : r->type);
	// printf("NODE_NAME: %s %s\n", ast_get_name_of(l), ast_get_name_of(r));
	// printf("DATA_TYPE, OP: %s %d %s\n", ast_get_type_name(ltype), op, ast_get_type_name(rtype));

	assert(ast_is_non_void_type(ltype));

	AST_NODE *node = NULL;

	// unary operator
	if(r == NO_NODE) {
		switch(op) {
			case LOGICAL_NOT:
				if(ltype == BOOL) {
					node = ast_create_node(EXPR_UNARY);
					goto success_follow_ltype;
				}
				yyerror("data type must be BOOL for LOGICAL NOT");
				return NULL;

			case BITWISE_NOT:
				if(ltype == INT) {
					node = ast_create_node(EXPR_UNARY);
					goto success_follow_ltype;
				}
				yyerror("data type must be INT for BITWISE NOT");
				return NULL;

			case NEGATIVE:
				if(ltype == INT || ltype == REAL) {
					node = ast_create_node(EXPR_UNARY);
					goto success_follow_ltype;
				}
				yyerror("data type must be INT or REAL for NEGATIVE");
				return NULL;
		}
	}

	assert(ast_is_non_void_type(rtype)); // check rtype is valid type

	// binary operator
	switch(op) {
		case LOGICAL_OR:
		case LOGICAL_AND:
			if(ltype != BOOL || rtype != BOOL) {
				yyerror("data type must be BOOL for LOGICAL OR/AND");
				return NULL;
			}
		case LT:
		case GT:
		case LTE:
		case GTE:
			if(ltype == STRING || rtype == STRING) {
				yyerror("can not use LT/GT/LTE/GTE to compare string");
				return NULL;
			}
		case EQ:
		case NEQ:
			if(ltype == rtype) {
				node = ast_create_node(EXPR_BINARY);
				node->expr.data_type = BOOL;
				goto success;
			}
			yyerror("compare type mismatch");
			return NULL;


		case BITWISE_OR:
		case BITWISE_AND:
			if(ltype != INT || rtype != INT) {
				yyerror("data type must be INT for BITWISE OR/AND");
				return NULL;
			}

		case ADD:
			if(ltype == STRING || rtype == STRING) {
				node = ast_create_node(EXPR_BINARY);
				node->expr.data_type = STRING;
				goto success;
			}
		case SUB:
		case XOR:
		case MUL:
		case DIV:
		case MOD:
			if(ltype != rtype) {
				yyerror("type mismatch in arithmetic opterator");
				return NULL;
			}
			if(ltype == BOOL) {
				yyerror("BOOL is not allowed in arithmetic operator");
				return NULL;
			}
			if(ltype == STRING) {
				yyerror("STRING is not allowed in arithmetic operator");
				return NULL;
			}
			node = ast_create_node(EXPR_BINARY);
			goto success_follow_ltype;
	}

success_follow_ltype:
	node->expr.data_type = ltype;
success:
	node->expr.op = op;
	node->expr.lval = l;
	node->expr.rval = r;
	return node;
}

AST_NODE* ast_create_if_node(AST_NODE* cond, AST_NODE *true_stmt,
		AST_NODE *false_stmt)
{
	if(ast_get_expr_type(cond) != BOOL) {
		yyerror("if condition must be BOOL");
		return NULL;
	}
	AST_NODE *node = ast_create_node(IF_STMT);
	node->if_stmt.cond = cond;
	node->if_stmt.true_stmt = true_stmt;
	node->if_stmt.false_stmt = false_stmt;
	return node;
}

AST_NODE* ast_create_for_node(AST_NODE* init, AST_NODE* cond,
		AST_NODE *increment, AST_NODE *body)
{
	AST_NODE *node = ast_create_node(FOR_STMT);
	if(ast_get_expr_type(cond) != BOOL) {
		yyerror("for condition must be BOOL");
		return NULL;
	}
	node->for_stmt.init = init;
	node->for_stmt.cond = cond;
	node->for_stmt.increment = increment;
	node->for_stmt.body = body;
	return node;
}

AST_NODE* ast_create_assign(AST_NODE *var_node, int idx, int op, AST_NODE *rval)
{
	if(var_node->type != VAR_DECL) {
		yyerror("lval in assignment must be a variable");
		return NULL;
	}

	int val_type = ast_get_expr_type(rval);
	if(!ast_is_non_void_type(val_type)) {
		yyerror("rval in assignment must be a non-void type");
		return NULL;
	}

	if(var_node->var.data_type != val_type) {
		yyerror("type mismatch in assignment");
		return NULL;
	}

	if(idx && idx >= var_node->var.array_size) {
		yyerror("array index out of bound in assignment");
		return NULL;
	}

	AST_NODE *node = ast_create_node(ASSIGN_STMT);
	node->assignment.lval = &var_node->var;
	node->assignment.rval = rval;

	return node;
}

int ast_get_expr_type(const AST_NODE *node)
{
	if(node == NULL || node == NO_NODE) {
		return VOID;
	}

	switch(node->type) {
		case CONST_VAL:
			return node->val.data_type;
		case VAR_DECL:
			return node->var.data_type;

		case FUNC_DECL:
			return node->func.return_type;
		case FUNC_CALL:
			return node->invoke.func->return_type;

		case ASSIGN_STMT:
			return node->assignment.lval->data_type;

		case PRINT_STMT:
		case READ_STMT:
			return VOID;

		case RETURN_STMT:
		case IF_STMT:
		case FOR_STMT:
			return VOID;

		case EXPR_UNARY:
		case EXPR_BINARY:
			return node->expr.data_type;

		default:
			assert(NULL); // Invalid node type
	}
}

const char * ast_get_name_of(const AST_NODE *node)
{
	if(node == NULL || node == NO_NODE) return NULL;
	switch(node->type) {
		case VAR_DECL:
			return node->var.symbol->name;
		case FUNC_DECL:
			return node->func.symbol->name;
		default:
			return NULL;
	}
}
