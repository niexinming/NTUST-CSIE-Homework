#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "ast.h"

void yyerror(const char *errmsg);

const struct AST_NODE_s NODE_CONST_STR_BR = {
	NULL, // next
	CONST_VAL, // type
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

AST_NODE* ast_create_node(enum AST_TYPE type)
{
	AST_NODE *ast_node = malloc(sizeof(AST_NODE));
	ast_node->next = NULL;
	ast_node->type = type;
	ast_node->child = NULL;
	return ast_node;
}

unsigned int ast_node_length(AST_NODE *node)
{
	if(node == NULL) {
		return -1;
	}
	unsigned int d = 0;
	while(node) {
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
	if(var == NULL) {
		puts("<ERROR(var is null)>");
		return;
	}

	if(var == NO_NODE) {
		puts("<VAR(NO_NODE)>");
		return;
	}

    const char *var_type = ast_get_var_type(var->var_type);
    const char *data_type = ast_get_type_name(var->data_type);
    printf("<%s(Name=%s, Type=%s", var_type, var->symbol->name, data_type);
    if(var->val) {
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
    printf(")>\n");
}

AST_NODE* ast_create_var_node(int data_type, int var_type,
		unsigned int array_size, SYMTAB_ENTRY *symbol,
		struct AST_VALUE_s * default_val)
{
	AST_NODE *node = ast_create_node(VAR_DECL);
    AST_VAR *var = &node->var;

    var->data_type = data_type;
    var->var_type = var_type;
    var->array_size = array_size;
    var->symbol = symbol;
    var->val = default_val;

    ast_dump_var(var);
    return node;
}

AST_NODE* ast_create_func_node(SYMTAB_ENTRY *symbol, int return_type,
		AST_NODE *params, AST_NODE *body)
{
	AST_NODE *node = ast_create_node(FUNC_DECL);
	AST_FUNC *func = &node->func;

	func->symbol = symbol;
	func->return_type = return_type;
	func->params = params;
	func->param_count = ast_node_length(params);
	func->body = body;

	printf("<FUNC(Name=%s, Type=%s, ParamCount=%d)>\n", symbol->name,
			ast_get_type_name(return_type), func->param_count);
	return node;
}

AST_NODE* ast_create_invoke_node(AST_FUNC *func, AST_NODE *args)
{
	AST_NODE *node = ast_create_node(FUNC_CALL);
	AST_INVOKE *invoke = &node->invoke;

	invoke->func = func;
	invoke->args = args;
	invoke->args_count = ast_node_length(args);

	printf("<INVOKE(Func=%s, ParamCount=%d)>\n", func->symbol->name,
			invoke->args_count);
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
					node = ast_create_node(EXPR_BINARY);
					goto success_follow_ltype;
				}
				yyerror("data type must be BOOL for LOGICAL NOT");
				return NULL;

			case BITWISE_NOT:
				if(ltype == INT) {
					node = ast_create_node(EXPR_BINARY);
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
