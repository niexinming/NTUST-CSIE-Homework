%{
#include <stdlib.h>
#include <stdio.h>

#include "symtab.h"
#include "ast.h"
#include "scanner.c"

int has_error = 0;
char err_msg_buf[1024];

void yyerror(const char *errmsg);
#define yyerrorf(...) sprintf(err_msg_buf, __VA_ARGS__); yyerror(err_msg_buf);

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
    struct AST_EXPR_s      *expr;
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

/* hack for constant definition */
%token PARAM

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
%type <ast_node> id_eval;

%type <integer> basic_type;
%type <data_type> type;

%type <ast_node> const_value;

%type <ast_node> var_decl;
%type <ast_node> const_decl;

%type <ast_node> expr;

%type <ast_node> func_params_list;
%type <ast_node> func_params_def;
%type <ast_node> func_decl;

%type <ast_node> invoke_args;
%type <ast_node> invoke_stmt;

%type <ast_node> block;

%%

/* atomic things */
id : ID {
    if(symtab_lookup($1, symtab, 0, NULL)) {
        yyerrorf("duplicated symbol: %s", $1);
    }

    if(symtab_insert($1, NULL, symtab, &$$) == 0) {
        yyerror("create symbol failed");
    }

    free($1);
};

id_eval : ID {
    SYMTAB_ENTRY *sym;
    if(symtab_lookup($1, symtab, 1, &sym) == 0) {
        yyerrorf("can not find symbol: %s", $1);
    }

    $$ = sym->meta;

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

const_value : CONST_INT    { $$ = ast_create_value_node(INT);     $$->val.integer = $1; }
            | CONST_REAL   { $$ = ast_create_value_node(REAL);    $$->val.real = $1; }
            | CONST_STRING { $$ = ast_create_value_node(STRING);  $$->val.string = strdup($1); }
            | TRUE         { $$ = ast_create_value_node(BOOL);    $$->val.integer = 1; }
            | FALSE        { $$ = ast_create_value_node(BOOL);    $$->val.integer = 0; }
            ;

/* variables */
var_decl : VAR id type {
             // data_type, var_type, array_size, symbol, default_val
             $$ = $2->meta = ast_create_var_node($3.t, VAR, $3.n, $2, NULL);
         }
         | VAR id type ASSIGN const_value
         {
             $$ = $2->meta = ast_create_var_node($3.t, VAR, $3.n, $2, &$5->val);
         }
         ;

const_decl : CONST id type ASSIGN const_value {
               if($3.t != $5->val.data_type) {
                   yyerrorf("Incompatable data type in const declaration: %s and %s",
                       ast_get_type_name($3.t), ast_get_type_name($5->val.data_type));
               }
               $$ = $2->meta = ast_create_var_node($3.t, CONST, $3.n, $2, &$5->val);
           }
           | CONST id ASSIGN const_value {
               $$ = $2->meta = ast_create_var_node($4->val.data_type, CONST, 0, $2, &$4->val);
           }
           ;

/* expressions */
expr : LEFT_PARENTHESIS expr RIGHT_PARENTHESIS { $$ = $2; }
     | const_value { $$ = ast_create_node(CONST_VAL); }
     | id_eval { $$ = ast_create_node(VAR_REF); }
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
assign : id_eval ASSIGN     expr { trace("assign value to var %s", $1->var.symbol->name); }
       | id_eval ASSIGN_ADD expr
       | id_eval ASSIGN_SUB expr
       | id_eval ASSIGN_MUL expr
       | id_eval ASSIGN_DIV expr
       | id_eval ASSIGN_MOD expr
       | id_eval ASSIGN_XOR expr
       ;

return_stmt : RETURN
            | RETURN expr;

invoke_args : expr { $$ = $1; }
            | expr COMMA invoke_args { $1->next = $3; $$ = $1; }
            | { $$ = NULL; }
            ;

invoke_stmt : id_eval LEFT_PARENTHESIS invoke_args RIGHT_PARENTHESIS {
                // TODO: create invoke stmt
                if($1->type != FUNC_DECL) {
                    yyerrorf("ID: %s is not a function", ast_get_name_of($1));
                    $$ = NULL;
                } else {
                    $$ = ast_create_invoke_node(&$1->func, $3);
                    trace("Invoke function : %s", ast_get_name_of($1));
                }
            };

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

func_params_list : id type {
                     $$ = $1->meta = ast_create_var_node($2.t, PARAM, $2.n, $1, NULL);
                 }
                 | id type COMMA func_params_list {
                     $$ = $1->meta = ast_create_var_node($2.t, PARAM, $2.n, $1, NULL);
                     $$->next = $4;
                 }
                 ;

func_params_def : func_params_list { $$ = $1; }
              | { $$ = NULL; }
              ;

begin_func : LEFT_PARENTHESIS { begin_context(); trace("begin of func"); };

func_decl :
    FUNC type id begin_func func_params_def RIGHT_PARENTHESIS
    block {
        end_context();
        trace("end of func");

        $$ = $3->meta = ast_create_func_node($3, $2.t, $5, $7);
        // TODO: check return type
    }
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
