#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "ast.h"

const char * ast_get_type_name(int typecode)
{
	switch(typecode) {
		case VOID: return "VOID";
		case INT: return "INT";
		case BOOL: return "BOOL";
		case REAL: return "REAL";
		case STRING: return "STRING";
		default: return "UNKNOWN";
	}
}

const char * ast_get_var_type(int var_type)
{
	switch (var_type) {
		case CONST: return "CONST";
		case VAR: return "VAR";
		case PARAM: return "PARAM";
		default: return "UNKNOWN";
	}
}

AST_NODE* ast_create_node(enum AST_TYPE type)
{
	AST_NODE *ast_node = malloc(sizeof(AST_NODE));
	ast_node->next = NULL;
	ast_node->type = type;
	return ast_node;
}

unsigned int ast_node_length(AST_NODE *node)
{
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
    AST_VALUE *val = &node->val;
    val->data_type = type;
    return node;
}

void ast_dump_var(AST_VAR* var)
{
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

AST_NODE* ast_create_expr_node(int oper, int data_type, AST_NODE *lval,
		AST_NODE* rval)
{
	AST_NODE *node = ast_create_node(rval == NULL ? EXPR_UNARY : EXPR_BINARY);
	AST_EXPR *expr = &node->expr;

	expr->data_type = data_type;
	expr->oper = oper;
	expr->lval = lval;
	expr->rval = rval;

	return node;
}

int ast_get_expr_type(AST_NODE *node)
{
	switch(node->type) {
		case CONST_VAL:
			return node->var.data_type;
		case CONST_DECL:
			return node->val.data_type;

		case VAR_REF:
			return node->var.data_type;
		case VAR_DECL:
			return node->var.data_type;

		case FUNC_DECL:
			return node->func.return_type;
		case FUNC_CALL:
			return node->invoke.func->return_type;

		case ASSIGN_STMT:
			return node->assignment.lval->data_type;
		case ASSIGN_ARRAY_STMT:
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

const char * ast_get_name_of(AST_NODE *node)
{
	switch(node->type) {
		case CONST_DECL:
		case VAR_DECL:
			return node->var.symbol->name;
		case FUNC_DECL:
			return node->func.symbol->name;
		default:
			return NULL;
	}
}
