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

	EXPR_STMT,
};

enum EXPR_TYPE {
	EXPR_CONST, EXPR_VAR, EXPR_UNARY, EXPR_BINARY
};

// value
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
	int                 var_type;
	unsigned int        array_size; // zero for non-array variable
	SYMTAB_ENTRY        *symbol;
	struct AST_VALUE_s  *val;
	struct AST_VAR_s    *next; // for param
} AST_VAR;

// expression
typedef struct AST_EXPR_s {
	enum EXPR_TYPE          type;
	int                     data_type;
	int                     oper;
	union {
		void                *lptr;
		struct AST_VALUE_s  *con;
		struct AST_VAR_s    *var;
		struct AST_EXPR_s   *lval;
		struct AST_INVOKE_s *call;
	};
	union {
		void                *rptr;
		struct AST_EXPR_s   *rval;
	};
} AST_EXPR;

// function parameter
typedef struct AST_FUNC_s {
	struct SYMTAB_ENTRY_s *symbol;
	int                   return_type;
	unsigned int          param_count;
	struct AST_VAR_s      *params;
	struct AST_NODE_s     *body;
} AST_FUNC;

// function call
typedef struct AST_INVOKE_s {
	struct SYMTAB_ENTRY_s *symbol;
	unsigned int          arg_count;
	struct AST_EXPR_s     *args;
} AST_INVOKE;

// assignment
typedef struct AST_ASSIGN_s {
	int               index;
	struct AST_VAR_s  *lval;
	struct AST_EXPR_s *rval;
} AST_ASSIGN;

typedef struct AST_NODE_s {
	struct AST_NODE_s       *next;
	enum AST_TYPE           type;
	union {
		void                *ptr;
		struct AST_VALUE_s  *value;
		struct AST_VAR_s    *var;
		struct AST_FUNC_s   *func;
		struct AST_INVOKE_s *invoke;
		struct AST_ASSIGN_s *assignment;
		struct AST_EXPR_s   *expr;
	};
} AST_NODE;

const char * ast_get_type_name(int);
AST_VALUE* ast_create_value(int);
AST_VAR* ast_create_var(int, int, unsigned int, struct SYMTAB_ENTRY_s *, struct AST_VALUE_s *);
AST_NODE* ast_create_node(enum AST_TYPE, void *);
AST_FUNC* ast_create_function(struct SYMTAB_ENTRY_s *symbol,
		int return_type, struct AST_VAR_s *params,
		struct AST_NODE_s *body);
AST_NODE* ast_create_expr_node(enum EXPR_TYPE type, int data_type,
		int oper, void *lptr, void *rptr);
int ast_get_expr_type(AST_NODE *);

#endif
