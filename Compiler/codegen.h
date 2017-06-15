#ifndef _CODEGEN_
#define _CODEGEN_

#include "ast.h"

void codegen(AST_NODE *prog);
void emit_label(const char * prefix, int lblid, const char *subid);
void emit_label_def(int lblid, const char *subid);
int emit_val(const AST_VALUE *val);
void emit_store(const AST_VAR *var);
void emit_func_param(const AST_FUNC *func);
int emit_invoke(const AST_INVOKE *invoke);
void emit_stmts(const AST_NODE *stmt);
int emit_expr(const AST_NODE *expr);

#endif
