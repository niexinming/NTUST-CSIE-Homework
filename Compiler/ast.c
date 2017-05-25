#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

AST_VALUE* ast_create_value(int type)
{
    AST_VALUE *val = malloc(sizeof(AST_VALUE));
    val->type = type;
    return val;
}

void ast_dump_var(AST_VAR* var)
{
    const char *var_type = "ERROR";
    if(var->var_type == CONST) var_type = "CONST";
    if(var->var_type == VAR) var_type = "VAR";
	if(var->var_type == ARG) var_type = "ARG";
    const char *t = "UNKNOW";
#define VAR_DUMP_CASE(C) case C: t = #C; break;
    switch (var->data_type) {
        VAR_DUMP_CASE(VOID);
        VAR_DUMP_CASE(INT);
        VAR_DUMP_CASE(REAL);
        VAR_DUMP_CASE(STRING);
        VAR_DUMP_CASE(BOOL);
    }
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

AST_VAR* ast_create_var(int type, int var_type, unsigned int array_size, SYMTAB_ENTRY *symbol, struct AST_VALUE_s * val)
{
    AST_VAR *var = malloc(sizeof(AST_VAR));
    var->data_type = type;
    var->var_type = var_type;
    var->array_size = array_size;
    var->symbol = symbol;
    var->val = val;
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

AST_NODE* ast_create_node_const(AST_VALUE *val)
{
	return ast_create_node(CONST_VAL, val);
}
