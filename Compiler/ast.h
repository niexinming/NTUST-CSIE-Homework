#ifndef AST_H
#define AST_H

#include "symtab.h"
#include "parser.h"

#define NEWLINE 1

enum AST_TYPE {
	// literal value
	CONST_VAL,

	// variable or function arguments
	VAR_DECL,

	// function define and call
	FUNC_DECL,
	FUNC_CALL,

	// assignment
	ASSIGN_STMT,

	// first-class special statment for IO
	PRINT_STMT,
	READ_STMT,

	// control flow
	RETURN_STMT,
	IF_STMT,
	FOR_STMT,

	// expression
	EXPR_UNARY,
	EXPR_BINARY,
};

// constant value
typedef struct AST_VALUE_s {
	int            data_type; // VOID, INT, BOOL, STRING, REAL
	union {
		const char *string;
		int        integer;
		double     real;
	};
} AST_VALUE;

// variable or paramteter or const
typedef struct AST_VAR_s {
	int                 data_type;
	int                 var_type;   // VAR, CONST, PARAM
	unsigned int        array_size; // zero for non-array variable
	int                 idx;
	SYMTAB_ENTRY        *symbol;    // var name
	struct AST_VALUE_s  *val;       // default value
} AST_VAR;

// function declaration
typedef struct AST_FUNC_s {
	struct SYMTAB_ENTRY_s   *symbol;
	int                     return_type;
	unsigned int            param_count;
	const struct AST_NODE_s *params;
	const struct AST_NODE_s *body;
} AST_FUNC;

// function call
typedef struct AST_INVOKE_s {
	const struct AST_FUNC_s *func;
	unsigned int            args_count;
	const struct AST_NODE_s *args;
} AST_INVOKE;

// assignment
typedef struct AST_ASSIGN_s {
	int                     index; // array index, 0 for non-array
	const struct AST_VAR_s  *lval; // which variable to write
	const struct AST_NODE_s *rval; // what to write
} AST_ASSIGN;

// expression
typedef struct AST_EXPR_s {
	int                     data_type;
	int                     op;
	const struct AST_NODE_s *lval;
	const struct AST_NODE_s *rval;
} AST_EXPR;

typedef struct AST_IF_s {
	const struct AST_NODE_s *cond;
	const struct AST_NODE_s *true_stmt;
	const struct AST_NODE_s *false_stmt;
} AST_IF;

typedef struct AST_FOR_s {
	const struct AST_NODE_s *init;
	const struct AST_NODE_s *cond;
	const struct AST_NODE_s *increment;
	const struct AST_NODE_s *body;
} AST_FOR;

// basic ast node
typedef struct AST_NODE_s {
	struct AST_NODE_s       *next;
	struct AST_NODE_s       *next_stmt;
	enum AST_TYPE           type;
	int                     flag;
	union {
		struct AST_VALUE_s  val;
		struct AST_VAR_s    var;
		struct AST_FUNC_s   func;
		struct AST_INVOKE_s invoke;
		struct AST_ASSIGN_s assignment;
		struct AST_EXPR_s   expr;
		struct AST_IF_s     if_stmt;
		struct AST_FOR_s    for_stmt;
		struct AST_NODE_s   *child;
	};
} AST_NODE;

#define NO_NODE ((void*)-1)

void ast_dump_str(const char *str);
const char * ast_get_type_name(int typecode);
const char * ast_get_node_type(enum AST_TYPE t);
AST_NODE* ast_create_node(enum AST_TYPE type);
const char * ast_get_op_str(int op);
unsigned int ast_node_length(AST_NODE *node);
AST_NODE* ast_create_value_node(int type);
void ast_dump_var(const AST_VAR* var);
AST_NODE* ast_create_var_node(int data_type, int var_type,
		unsigned int array_size, SYMTAB_ENTRY *symbol,
		struct AST_VALUE_s * default_val);
AST_NODE* ast_create_func_node(SYMTAB_ENTRY *symbol, int return_type,
		AST_NODE *params, AST_NODE *body);
AST_NODE* ast_create_invoke_node(AST_FUNC *func, AST_NODE *args);
void ast_dump_func_call(const AST_INVOKE *invoke);
void ast_dump_expr(const AST_NODE *node);
void ast_dump_stmt(const AST_NODE *stmt);
void ast_dump_stmt_body(const AST_NODE *p);
AST_NODE* ast_create_expr_node(const AST_NODE *l, int op, const AST_NODE *r);
AST_NODE* ast_create_if_node(AST_NODE* cond, AST_NODE *true_stmt,
		AST_NODE *false_stmt);
AST_NODE* ast_create_for_node(AST_NODE* init, AST_NODE* cond,
		AST_NODE *increment, AST_NODE *body);
AST_NODE* ast_create_assign(AST_NODE *var, int idx, int op, AST_NODE *val);
int ast_get_expr_type(const AST_NODE *node);
const char * ast_get_name_of(const AST_NODE *node);

extern const struct AST_NODE_s NODE_CONST_STR_BR;

#endif
