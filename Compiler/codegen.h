#ifndef _CODEGEN_
#define _CODEGEN_

#include "ast.h"

void codegen(AST_NODE *prog);
void emit_val(const AST_VALUE *val);
void emit_store(const AST_VAR *var);
void emit_func_param(AST_FUNC *func);

#endif
