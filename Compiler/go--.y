%{
#include <stdlib.h>
#include <stdio.h>

#include "symtab.h"
#include "ast.h"
#include "scanner.c"

void yyerror(const char *errmsg);

#define trace(...) printf("TRACE: "); printf(__VA_ARGS__); putchar(0x0a)

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
    void*                  ast_node;
    struct { int t, n; }   data_type;
    struct AST_VAR_s       *var;
}

/* tokens definition */

/* basic identifier and constants */
%token <symbol>  ID
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

%%

/* atomic things */
id : ID;

id_eval : ID { $$ = $1; };

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

const_value : CONST_INT    { $$ = ast_new_value(INT);     $$->integer = $1; }
            | CONST_REAL   { $$ = ast_new_value(REAL);    $$->real = $1; }
            | CONST_STRING { $$ = ast_new_value(STRING);  $$->string = strdup($1); }
            | TRUE         { $$ = ast_new_value(BOOL);    $$->integer = 1; }
            | FALSE        { $$ = ast_new_value(BOOL);    $$->integer = 0; }
            ;

/* variables */
var_decl : VAR id type {
             $$ = ast_new_var($3.t, VAR, $3.n, $2, NULL);
         }
         | VAR id type ASSIGN const_value
         {
             $$ = ast_new_var($3.t, VAR, $3.n, $2, $5);
         }
         ;

const_decl : CONST id type ASSIGN const_value { $$ = ast_new_var($3.t, CONST, $3.n, $2, $5); }
           | CONST id ASSIGN const_value      { $$ = ast_new_var($4->type, CONST, 0, $2, $4); }
           ;

/* expressions */
expr : LEFT_PARENTHESIS expr RIGHT_PARENTHESIS { trace("expr -> LEFT_PARENTHESIS expr RIGHT_PARENTHESIS"); }
     | const_value                             { trace("expr -> const_value                            "); }
     | id_eval                                 { trace("expr -> id_eval (%s)                           ", $1->name); }
     | invoke_stmt                             { trace("expr -> invoke_stmt                            "); }
     | expr LOGICAL_OR expr                    { trace("expr -> expr LOGICAL_OR expr                   "); }
     | expr LOGICAL_AND expr                   { trace("expr -> expr LOGICAL_AND expr                  "); }
     | expr LOGICAL_NOT expr                   { trace("expr -> expr LOGICAL_NOT expr                  "); }
     | expr LT expr                            { trace("expr -> expr LT expr                           "); }
     | expr GT expr                            { trace("expr -> expr GT expr                           "); }
     | expr LTE expr                           { trace("expr -> expr LTE expr                          "); }
     | expr GTE expr                           { trace("expr -> expr GTE expr                          "); }
     | expr EQ expr                            { trace("expr -> expr EQ expr                           "); }
     | expr NEQ expr                           { trace("expr -> expr NEQ expr                          "); }
     | expr ADD expr                           { trace("expr -> expr ADD expr                          "); }
     | expr SUB expr                           { trace("expr -> expr SUB expr                          "); }
     | expr BITWISE_OR expr                    { trace("expr -> expr BITWISE_OR expr                   "); }
     | expr BITWISE_AND expr                   { trace("expr -> expr BITWISE_AND expr                  "); }
     | expr XOR expr                           { trace("expr -> expr XOR expr                          "); }
     | expr MUL expr                           { trace("expr -> expr MUL expr                          "); }
     | expr DIV expr                           { trace("expr -> expr DIV expr                          "); }
     | expr MOD expr                           { trace("expr -> expr MOD expr                          "); }
     | BITWISE_NOT expr                        { trace("expr -> BITWISE_NOT expr                       "); }
     | LOGICAL_NOT expr                        { trace("expr -> LOGICAL_NOT expr                       "); }
     | SUB expr                                { trace("expr -> SUB expr                               "); }
     ;



/* basic stmt */
assign : id ASSIGN     expr { trace("assign value to var %s", $1->name); }
       | id ASSIGN_ADD expr
       | id ASSIGN_SUB expr
       | id ASSIGN_MUL expr
       | id ASSIGN_DIV expr
       | id ASSIGN_MOD expr
       | id ASSIGN_XOR expr
       ;

return_stmt : RETURN
            | RETURN expr;

invoke_args : expr
            | invoke_args COMMA expr
            |
            ;

invoke_stmt : id LEFT_PARENTHESIS invoke_args RIGHT_PARENTHESIS { trace("Invoke function : %s", $1->name); };

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
block : LEFT_BRACE stmts RIGHT_BRACE;

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

for_init_stmt : stmt_set;

for_cond : expr;

for_inc : stmt_set;

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
         ;

/* functions */

func_args_list : id type
               | func_args_list COMMA id type;

func_args_def : func_args_list
              |
              ;

func_decl :
    FUNC type id LEFT_PARENTHESIS func_args_def RIGHT_PARENTHESIS
    block
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
    printf("[-] (%d, %d): %s\n", linenum, colnum, errmsg);
}

/* vim: et */
