#include <stdlib.h>
#include "ast.h"

AST_VALUE* ast_new_value(int type)
{
    AST_VALUE *val = malloc(sizeof(AST_VALUE));
    val->type = type;
    return val;
}

void ast_dump_var(AST_VAR* var)
{
    const char *const_or_var = "ERROR";
    if(var->is_const == CONST) const_or_var = "CONST";
    if(var->is_const == VAR) const_or_var = "VAR";
    const char *t = "UNKNOW";
#define VAR_DUMP_CASE(C) case C: t = #C; break;
    switch (var->type) {
        VAR_DUMP_CASE(VOID);
        VAR_DUMP_CASE(INT);
        VAR_DUMP_CASE(REAL);
        VAR_DUMP_CASE(STRING);
        VAR_DUMP_CASE(BOOL);
    }
    printf("<%s(Name=%s, Type=%s", const_or_var, var->symbol->name, t);
    if(var->val) {
        printf(", Value=");
        switch (var->type) {
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

AST_VAR* ast_new_var(int type, int is_const, unsigned int array_size, SYMTAB_ENTRY *symbol, struct AST_VALUE_s * val)
{
    AST_VAR *var = malloc(sizeof(AST_VAR));
    var->type = type;
    var->is_const = is_const;
    var->array_size = array_size;
    var->symbol = symbol;
    var->val = val;
    ast_dump_var(var);
    return var;
}
