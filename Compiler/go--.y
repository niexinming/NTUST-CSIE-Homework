%{
#include <stdlib.h>
#include <stdio.h>

#include "symtab.h"
#include "ast.h"
#include "scanner.c"

int has_error = 0;
char err_msg_buf[1024];
void yyerror(const char *errmsg);

#define trace(...) printf("TRACE: "); printf(__VA_ARGS__); putchar(0x0a)

SYMTAB *symtab, *root_symtab;

void begin_context() {
    symtab = symtab_create(symtab);
}

void end_context() {
    symtab_dump(symtab);
    symtab = symtab_destroy(symtab);
}

%}

/* this language starts with a `program` */

%start program

/* yylval union type */

%union {
    struct SYMTAB_ENTRY_s  *symbol;
    struct AST_VALUE_s     *value;
    int                    integer;
    double                 real;
    char*                  string;
    struct AST_NODE_s      *ast_node;
    struct { int t, n; }   data_type;
    struct AST_VAR_s       *var;
}

/* tokens definition */

/* basic identifier and constants */
%token <string>  ID
%token <integer> CONST_INT
%token <real>    CONST_REAL
%token <string>  CONST_STRING
/* data types */
%token BOOL INT STRING REAL

%token COMMA COLON SEMICOLON LEFT_PARENTHESIS RIGHT_PARENTHESIS LEFT_BRACKET RIGHT_BRACKET LEFT_BRACE RIGHT_BRACE
%token LOGICAL_NOT BITWISE_NOT
%token ADD SUB MUL DIV MOD XOR BITWISE_AND BITWISE_OR LOGICAL_AND LOGICAL_OR
/*
%token LT GT LTE GTE EQ NEQ
%token ASSIGN ASSIGN_ADD ASSIGN_SUB ASSIGN_MUL ASSIGN_DIV ASSIGN_MOD ASSIGN_XOR
*/
%token BREAK CASE CONST CONTINUE DEFAULT ELSE FALSE FOR FUNC GO IF IMPORT NIL PRINT PRINTLN RETURN STRUCT SWITCH TRUE TYPE VAR VOID WHILE

/* operator priority */

%left LOGICAL_OR
%left LOGICAL_AND
%left LOGICAL_NOT
%left LT GT LTE GTE EQ NEQ
%left ADD SUB BITWISE_OR BITWISE_AND
%left MUL DIV MOD
%left XOR
%left BITWISE_NOT

%nonassoc ASSIGN ASSIGN_ADD ASSIGN_SUB ASSIGN_MUL ASSIGN_DIV ASSIGN_MOD ASSIGN_XOR

%type <symbol> id;
%type <symbol> id_eval;

%type <integer> basic_type;
%type <data_type> type;

%type <value> const_value;

%type <var> var_decl;
%type <var> const_decl;

%type <ast_node> expr;

%%

/* atomic things */
id : ID {
    if(symtab_lookup($1, symtab, 0, NULL)) {
        sprintf(err_msg_buf, "duplicated symbol: %s", $1);
        yyerror(err_msg_buf);
    }

    if(symtab_insert($1, NULL, symtab, &$$) == 0) {
        yyerror("create symbol failed");
    }

    free($1);
};

id_eval : ID {
    if(symtab_lookup($1, symtab, 1, &$$) == 0) {
        sprintf(err_msg_buf, "can not find symbol: %s", $1);
        yyerror(err_msg_buf);
    }

    free($1);
};

/* basic immutable data types */
basic_type : STRING { $$ = STRING; }
           | INT    { $$ = INT; }
           | REAL   { $$ = REAL; }
           | BOOL   { $$ = BOOL; }
           ;

/* array of basic_type, basic_type, void */
type : LEFT_BRACKET CONST_INT RIGHT_BRACKET basic_type { $$.t = $4; $$.n = $2; }
     | basic_type { $$.n = 0; $$.t = $1; }
     | VOID { $$.n = 0; $$.t = VOID; }
     ;

const_value : CONST_INT    { $$ = ast_create_value(INT);     $$->integer = $1; }
            | CONST_REAL   { $$ = ast_create_value(REAL);    $$->real = $1; }
            | CONST_STRING { $$ = ast_create_value(STRING);  $$->string = strdup($1); }
            | TRUE         { $$ = ast_create_value(BOOL);    $$->integer = 1; }
            | FALSE        { $$ = ast_create_value(BOOL);    $$->integer = 0; }
            ;

/* variables */
var_decl : VAR id type {
             $$ = ast_create_var($3.t, VAR, $3.n, $2, NULL);
         }
         | VAR id type ASSIGN const_value
         {
             $$ = ast_create_var($3.t, VAR, $3.n, $2, $5);
         }
         ;

const_decl : CONST id type ASSIGN const_value { $$ = ast_create_var($3.t, CONST, $3.n, $2, $5); }
           | CONST id ASSIGN const_value      { $$ = ast_create_var($4->type, CONST, 0, $2, $4); }
           ;

/* expressions */
expr : LEFT_PARENTHESIS expr RIGHT_PARENTHESIS { $$ = $2; }
     | const_value { $$ = ast_create_node(CONST_VAL, $1); }
     | id_eval { $$ = ast_create_node(VAR_DECL, NULL); }
     | invoke_stmt
     | expr LOGICAL_OR expr
     | expr LOGICAL_AND expr
     | expr LOGICAL_NOT expr
     | expr LT expr
     | expr GT expr
     | expr LTE expr
     | expr GTE expr
     | expr EQ expr
     | expr NEQ expr
     | expr ADD expr
     | expr SUB expr
     | expr BITWISE_OR expr
     | expr BITWISE_AND expr
     | expr XOR expr
     | expr MUL expr
     | expr DIV expr
     | expr MOD expr
     | BITWISE_NOT expr
     | LOGICAL_NOT expr
     | SUB expr
     ;



/* basic stmt */
assign : id_eval ASSIGN     expr { trace("assign value to var %s", $1->name); }
       | id_eval ASSIGN_ADD expr
       | id_eval ASSIGN_SUB expr
       | id_eval ASSIGN_MUL expr
       | id_eval ASSIGN_DIV expr
       | id_eval ASSIGN_MOD expr
       | id_eval ASSIGN_XOR expr
       ;

return_stmt : RETURN
            | RETURN expr;

invoke_args : expr
            | invoke_args COMMA expr
            |
            ;

invoke_stmt : id_eval LEFT_PARENTHESIS invoke_args RIGHT_PARENTHESIS { trace("Invoke function : %s", $1->name); };

stmt_set : assign { trace("assign in stmt"); }
         | var_decl { trace("var_decl in stmt"); }
         | const_decl { trace("const_decl in stmt"); }
         | if_stmt { trace("if_stmt in stmt"); }
         | print_stmt { trace("print in stmt"); }
         | for_stmt { trace("for in stmt"); }
         | return_stmt { trace("return in stmt"); }
         | invoke_stmt { trace("invoke in stmt"); }
         ;

stmt : stmt_set { trace("stmt"); } | stmt_set SEMICOLON { trace("stmt_with_semi"); };

stmts : stmt
      | stmts stmt
      ;

/* { ... code ... } */
begin_block : LEFT_BRACE { begin_context(); trace("begin of block"); };

end_block : RIGHT_BRACE { end_context(); trace("end of block"); };

block : begin_block stmts end_block;

/* special stmt */
print_stmt : PRINT expr
           | PRINTLN expr
           ;

/* control flow */
if_stmt : IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS
            if_body
        | IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS
            if_body
          ELSE
            if_body
        ;

if_body : block | stmt;

for_init_stmt : stmt_set
              |
              ;

for_cond : expr
         |
         ;

for_inc : stmt_set
        |
        ;

for_body : block | stmt;

for_stmt : FOR LEFT_PARENTHESIS
               for_init_stmt SEMICOLON
               for_cond SEMICOLON
               for_inc
           RIGHT_PARENTHESIS for_body
         | FOR LEFT_PARENTHESIS
               for_cond SEMICOLON
               for_inc
           RIGHT_PARENTHESIS for_body
         | FOR LEFT_PARENTHESIS
               for_cond
           RIGHT_PARENTHESIS for_body
         ;

/* functions */

func_args_list : id type
               | func_args_list COMMA id type;

func_args_def : func_args_list
              |
              ;

begin_func : LEFT_PARENTHESIS { begin_context(); trace("begin of func"); };

func_decl :
    FUNC type id begin_func func_args_def RIGHT_PARENTHESIS
    block { end_context(); trace("end of func"); }
    ;

/* merge them into program */
program : var_decl program
        | const_decl program
        | func_decl program
        |
        ;


%%
void yyerror(const char *errmsg)
{
    has_error = 1;
    printf("ERR (%d, %d): %s\n", linenum, colnum, errmsg);
}

int main()
{
	symtab = root_symtab = symtab_create(NULL);

	yyparse();

	printf("\n"
			"  =======================\n"
			"  == symbol table dump ==\n"
			"  =======================\n"
			"\n");

	symtab_dump(root_symtab);
	symtab_destroy(root_symtab);
}

/*
    vim: et
*/
