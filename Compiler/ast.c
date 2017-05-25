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
		default: return "UNKNOW";
	}
}

AST_VALUE* ast_create_value(int type)
{
    AST_VALUE *val = malloc(sizeof(AST_VALUE));
    val->data_type = type;
    return val;
}

void ast_dump_var(AST_VAR* var)
{
    const char *var_type = "ERROR";
    if(var->var_type == CONST) var_type = "CONST";
    if(var->var_type == VAR) var_type = "VAR";
	if(var->var_type == ARG) var_type = "ARG";
    const char *t = ast_get_type_name(var->data_type);
    printf("<%s(Name=%s, Type=%s", var_type, var->symbol->name, t);
    if(var->val) {
        printf(", Value=");
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

AST_VAR* ast_create_var(int type, int var_type,
		unsigned int array_size,
		SYMTAB_ENTRY *symbol, struct AST_VALUE_s * val)
{
    AST_VAR *var = malloc(sizeof(AST_VAR));
    var->data_type = type;
    var->var_type = var_type;
    var->array_size = array_size;
    var->symbol = symbol;
    var->val = val;
	var->next = NULL;
    ast_dump_var(var);
    return var;
}

AST_NODE* ast_create_node(enum AST_TYPE type, void *ptr)
{
	AST_NODE *ast_node = malloc(sizeof(AST_NODE));
	ast_node->next = NULL;
	ast_node->type = type;
	ast_node->ptr = ptr;
	return ast_node;
}

AST_FUNC* ast_create_function(struct SYMTAB_ENTRY_s *symbol,
		int return_type, struct AST_VAR_s *params,
		struct AST_NODE_s *body)
{
	AST_FUNC *func = malloc(sizeof(AST_FUNC));
	func->symbol = symbol;
	func->return_type = return_type;
	func->params = params;
	func->param_count = 0;
	while(params) {
		func->param_count++;
		params = params->next;
	}
	func->body = body;

	printf("<FUNC(Name=%s, Type=%s, ParamCount=%d)>\n",
			symbol->name, ast_get_type_name(return_type), func->param_count);
	return func;
}

AST_NODE* ast_create_expr_node(enum EXPR_TYPE type, int data_type,
		int oper, void *lptr, void *rptr)
{
	AST_EXPR *expr = malloc(sizeof(AST_EXPR));
	expr->type = type;
	expr->data_type = data_type;
	expr->oper = oper;
	expr->lptr = lptr;
	expr->rptr = rptr;
	return ast_create_node(EXPR_STMT, expr);
}

int ast_get_expr_type(AST_NODE *node)
{
	switch(node->type) {
		case CONST_VAL:
			return node->var->data_type;
		case CONST_DECL:
			return node->value->data_type;

		case VAR_REF:
			return node->var->data_type;
		case VAR_DECL:
			return node->var->data_type;

		case FUNC_DECL:
			return node->func->return_type;
		case FUNC_CALL:
			return node->func->return_type;

		case ASSIGN_STMT:
			return node->assignment->lval->data_type;
		case ASSIGN_ARRAY_STMT:
			return node->assignment->lval->data_type;

		case PRINT_STMT:
			return VOID;
		case READ_STMT:
			return VOID;

		case RETURN_STMT:
			return VOID;
		case IF_STMT:
			return VOID;
		case FOR_STMT:
			return VOID;
		case EXPR_STMT:
			return node->expr->data_type;
		default:
			assert(NULL); // Invalid node type
	}
}
