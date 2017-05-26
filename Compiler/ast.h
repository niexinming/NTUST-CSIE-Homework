#ifndef AST_H
#define AST_H

#include "symtab.h"
#include "parser.h"

enum AST_TYPE {
	// literal value
	CONST_VAL,

	// const define
	CONST_DECL,

	// variable or argument reference
	VAR_REF,

	// variable or function arguments
	VAR_DECL,

	// function define and call
	FUNC_DECL,
	FUNC_CALL,

	// assignment
	ASSIGN_STMT,
	ASSIGN_ARRAY_STMT,

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
	SYMTAB_ENTRY        *symbol;    // var name
	struct AST_VALUE_s  *val;       // default value
} AST_VAR;

// function declaration
typedef struct AST_FUNC_s {
	struct SYMTAB_ENTRY_s *symbol;
	int                   return_type;
	unsigned int          param_count;
	struct AST_NODE_s     *params;
	struct AST_NODE_s     *body;
} AST_FUNC;

// function call
typedef struct AST_INVOKE_s {
	struct AST_FUNC_s     *func;
	unsigned int          args_count;
	struct AST_NODE_s     *args;
} AST_INVOKE;

// assignment
typedef struct AST_ASSIGN_s {
	int               index; // array index, 0 for non-array
	struct AST_VAR_s  *lval; // which variable to write
	struct AST_NODE_s *rval; // what to write
} AST_ASSIGN;

// expression
typedef struct AST_EXPR_s {
	int               data_type;
	int               oper;
	struct AST_NODE_s *lval;
	struct AST_NODE_s *rval;
} AST_EXPR;

// basic ast node
typedef struct AST_NODE_s {
	struct AST_NODE_s       *next;
	enum AST_TYPE           type;
	union {
		struct AST_VALUE_s  val;
		struct AST_VAR_s    var;
		struct AST_FUNC_s   func;
		struct AST_INVOKE_s invoke;
		struct AST_ASSIGN_s assignment;
		struct AST_EXPR_s   expr;
	};
} AST_NODE;

const char * ast_get_type_name(int typecode);
AST_NODE* ast_create_node(enum AST_TYPE type);
unsigned int ast_node_length(AST_NODE *node);
AST_NODE* ast_create_value_node(int type);
void ast_dump_var(AST_VAR* var);
AST_NODE* ast_create_var_node(int data_type, int var_type,
		unsigned int array_size, SYMTAB_ENTRY *symbol,
		struct AST_VALUE_s * default_val);
AST_NODE* ast_create_func_node(SYMTAB_ENTRY *symbol, int return_type,
		AST_NODE *params, AST_NODE *body);
AST_NODE* ast_create_invoke_node(AST_FUNC *func, AST_NODE *args);
AST_NODE* ast_create_expr_node(int oper, int data_type, AST_NODE *lval,
		AST_NODE* rval);
int ast_get_expr_type(AST_NODE *node);
const char * ast_get_name_of(AST_NODE *node);

#endif
