#include "symtab.h"
#include "parser.h"

enum AST_TYPE {
	// literal value
	CONST_VAL,

	// const define
	CONST_DECL,

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
};

enum EXPR_TYPE {
	EXPR_CONST, EXPR_VAR, EXPR_UNARY, EXPR_BINARY
};

// value
typedef struct AST_VALUE_s {
	int            type; // VOID, INT, BOOL, STRING, REAL
	union {
		const char *string;
		int        integer;
		double     real;
	};
} AST_VALUE;

// variable or paramteter or const
typedef struct AST_VAR_s {
	int                type;
	int                is_const;
	unsigned int       array_size; // zero for non-array variable
	SYMTAB_ENTRY       *symbol;
	struct AST_VALUE_s *val;
} AST_VAR;

// expression
typedef struct AST_EXPR_s {
	enum EXPR_TYPE         type;
	int                    oper;
	union {
		struct AST_VALUE_s *con;
		struct AST_VAR_s   *var;
		struct AST_EXPR_s  *lval;
	};
	struct AST_EXPR_s      *rval;
} AST_EXPR;

// function parameter
typedef struct AST_FUNC_s {
	struct SYMTAB_ENTRY_s *name;
	unsigned int          param_count;
	struct AST_VAR_s      *params;
	struct AST_STMT_s     *stmts;
} AST_FUNC;

// function call
typedef struct AST_INVOKE_s {
	struct SYMTAB_ENTRY_s *name;
	unsigned int          arg_count;
	struct AST_EXPR_s     *args;
} AST_INVOKE;

// assignment
typedef struct AST_ASSIGN_s {
	struct SYMTAB_ENTRY_s *lval;
	struct AST_EXPR_s     *rval;
} AST_ASSIGN;

typedef struct AST_s {
	struct AST_s            *next;
	enum AST_TYPE           type;
	union {
		struct AST_VALUE_s  value;
		struct AST_VAR_s    var;
		struct AST_FUNC_s   func;
		struct AST_INVOKE_s invoke;
		struct AST_ASSIGN_s assignment;
	};
} AST_NODE;

AST_VALUE* ast_new_value(int);
AST_VAR* ast_new_var(int, int, unsigned int, SYMTAB_ENTRY *, struct AST_VALUE_s *);
