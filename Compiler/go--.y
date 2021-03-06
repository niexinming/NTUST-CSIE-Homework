%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strdup.h"

#include "symtab.h"
#include "ast.h"
#include "scanner.c"

int parser_trace = 0;
char err_msg_buf[1024];

void yyerror(const char *errmsg);
#define yyerrorf(...) sprintf(err_msg_buf, __VA_ARGS__); yyerror(err_msg_buf);

#define trace(...) if(getenv("TRACE") || parser_trace) {printf("TRACE: "); printf(__VA_ARGS__); puts("");}

SYMTAB *symtab, *root_symtab;
AST_NODE *prog = NO_NODE;

void begin_context() {
    trace("begin context");
    symtab = symtab_create(symtab);
}

void end_context() {
    //symtab_dump(symtab);
    //symtab = symtab_destroy(symtab);
    symtab = symtab->parent;
    trace("exit context");
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
/*

- in %left
%token ADD SUB MUL DIV MOD XOR BITWISE_AND BITWISE_OR LOGICAL_AND LOGICAL_OR
%token LT GT LTE GTE EQ NEQ

- in %nonassoc
%token ASSIGN ASSIGN_ADD ASSIGN_SUB ASSIGN_MUL ASSIGN_DIV ASSIGN_MOD ASSIGN_XOR
*/
%token BREAK CASE CONST CONTINUE DEFAULT ELSE FALSE FOR FUNC GO IF IMPORT NIL PRINT PRINTLN READ RETURN STRUCT SWITCH TRUE TYPE VAR VOID WHILE

/* hack for constant definition */
%token PARAM NEGATIVE NOP STREQU STRCAT STRVAL ARRGET

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

%type <ast_node> program;

%type <symbol> id;
%type <symbol> id_new;
%type <ast_node> id_eval;

%type <integer> basic_type;
%type <data_type> type;

%type <ast_node> const_value;

%type <ast_node> var_decl;
%type <ast_node> const_decl;

%type <ast_node> expr;
%type <ast_node> assign;
%type <ast_node> return_stmt;

%type <ast_node> func_params_list;
%type <ast_node> func_params_def;
%type <ast_node> func_type_id;
%type <ast_node> func_head;
%type <ast_node> func_decl;

%type <ast_node> invoke_args;
%type <ast_node> invoke_stmt;

%type <ast_node> stmt_set;
%type <ast_node> stmt;
%type <ast_node> stmts;
%type <ast_node> block;

%type <ast_node> print_stmt;
%type <ast_node> read_stmt;

%type <ast_node> if_stmt;
%type <ast_node> if_body;



%type <ast_node> for_init_stmt;
%type <ast_node> for_cond;
%type <ast_node> for_inc;
%type <ast_node> for_body;
%type <ast_node> for_stmt;


%%

/* atomic things */
id : ID {
    if(symtab_lookup($1, symtab, 0, NULL)) {
        yyerrorf("duplicated symbol: %s", $1);
    }

    if(symtab_insert($1, NO_NODE, symtab, &$$) == 0) {
        yyerror("create symbol failed");
        exit(1);
    }

    trace("id var: %s", $1);

    free($1);
};

id_new : ID {
    if(symtab_insert($1, NO_NODE, symtab, &$$) == 0) {
        yyerror("create symbol failed");
        exit(1);
    }

    trace("id_new var: %s", $1);

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
            | CONST_STRING { $$ = ast_create_value_node(STRING);  $$->val.string = mystrdup($1); }
            | TRUE         { $$ = ast_create_value_node(BOOL);    $$->val.integer = 1; }
            | FALSE        { $$ = ast_create_value_node(BOOL);    $$->val.integer = 0; }
            ;

/* variables */
var_decl : VAR id type {
             // data_type, var_type, array_size, symbol, default_val
             $$ = $2->meta = ast_create_var_node($3.t, VAR, $3.n, $2, NO_NODE);
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
     | const_value           { $$ = $1; }
     | id_eval               { $$ = $1; }
     | invoke_stmt           { $$ = $1; }
     | expr LOGICAL_OR expr  { $$ = ast_create_expr_node($1, LOGICAL_OR,  $3); }
     | expr LOGICAL_AND expr { $$ = ast_create_expr_node($1, LOGICAL_AND, $3); }
     | expr LT expr          { $$ = ast_create_expr_node($1, LT,          $3); }
     | expr GT expr          { $$ = ast_create_expr_node($1, GT,          $3); }
     | expr LTE expr         { $$ = ast_create_expr_node($1, LTE,         $3); }
     | expr GTE expr         { $$ = ast_create_expr_node($1, GTE,         $3); }
     | expr EQ expr          { $$ = ast_create_expr_node($1, EQ,          $3); }
     | expr NEQ expr         { $$ = ast_create_expr_node($1, NEQ,         $3); }
     | expr BITWISE_OR expr  { $$ = ast_create_expr_node($1, BITWISE_OR,  $3); }
     | expr BITWISE_AND expr { $$ = ast_create_expr_node($1, BITWISE_AND, $3); }
     | expr ADD expr         { $$ = ast_create_expr_node($1, ADD,         $3); }
     | expr SUB expr         { $$ = ast_create_expr_node($1, SUB,         $3); }
     | expr XOR expr         { $$ = ast_create_expr_node($1, XOR,         $3); }
     | expr MUL expr         { $$ = ast_create_expr_node($1, MUL,         $3); }
     | expr DIV expr         { $$ = ast_create_expr_node($1, DIV,         $3); }
     | expr MOD expr         { $$ = ast_create_expr_node($1, MOD,         $3); }
     | LOGICAL_NOT expr      { $$ = ast_create_expr_node($2, LOGICAL_NOT, NO_NODE); }
     | BITWISE_NOT expr      { $$ = ast_create_expr_node($2, BITWISE_NOT, NO_NODE); }
     | SUB expr              { $$ = ast_create_expr_node($2, NEGATIVE,    NO_NODE); }
     | id_eval LEFT_BRACKET expr RIGHT_BRACKET { $$ = ast_create_expr_node($1, ARRGET, $3); }
     ;



/* basic stmt */
assign : id_eval ASSIGN     expr { $$ = ast_create_assign($1, NO_NODE, NOP, $3); }
       | id_eval ASSIGN_ADD expr { $$ = ast_create_assign($1, NO_NODE, ADD, $3); }
       | id_eval ASSIGN_SUB expr { $$ = ast_create_assign($1, NO_NODE, SUB, $3); }
       | id_eval ASSIGN_MUL expr { $$ = ast_create_assign($1, NO_NODE, MUL, $3); }
       | id_eval ASSIGN_DIV expr { $$ = ast_create_assign($1, NO_NODE, DIV, $3); }
       | id_eval ASSIGN_MOD expr { $$ = ast_create_assign($1, NO_NODE, MOD, $3); }
       | id_eval ASSIGN_XOR expr { $$ = ast_create_assign($1, NO_NODE, XOR, $3); }
       | id_eval LEFT_BRACKET expr RIGHT_BRACKET ASSIGN     expr { $$ = ast_create_assign($1, $3, NOP, $6); }
       | id_eval LEFT_BRACKET expr RIGHT_BRACKET ASSIGN_ADD expr { $$ = ast_create_assign($1, $3, ADD, $6); }
       | id_eval LEFT_BRACKET expr RIGHT_BRACKET ASSIGN_SUB expr { $$ = ast_create_assign($1, $3, SUB, $6); }
       | id_eval LEFT_BRACKET expr RIGHT_BRACKET ASSIGN_MUL expr { $$ = ast_create_assign($1, $3, MUL, $6); }
       | id_eval LEFT_BRACKET expr RIGHT_BRACKET ASSIGN_DIV expr { $$ = ast_create_assign($1, $3, DIV, $6); }
       | id_eval LEFT_BRACKET expr RIGHT_BRACKET ASSIGN_MOD expr { $$ = ast_create_assign($1, $3, MOD, $6); }
       | id_eval LEFT_BRACKET expr RIGHT_BRACKET ASSIGN_XOR expr { $$ = ast_create_assign($1, $3, XOR, $6); }
       ;

return_stmt : RETURN { $$ = ast_create_node(RETURN_STMT); }
            | RETURN expr { $$ = ast_create_node(RETURN_STMT); $$->child = $2; }
            ;

invoke_args : expr { $$ = $1; $$->next = NO_NODE; }
            | expr COMMA invoke_args { $1->next = $3; $$ = $1; }
            | { $$ = NO_NODE; }
            ;

invoke_stmt : id_eval LEFT_PARENTHESIS invoke_args RIGHT_PARENTHESIS {
                if($1 == NO_NODE) {
                    yyerror("Recursion not supported");
                }
                if($1->type != FUNC_DECL) {
                    yyerrorf("ID: %s is not a function", ast_get_name_of($1));
                    $$ = NULL;
                } else {
                    $$ = ast_create_invoke_node(&$1->func, $3);
                    trace("Invoke function : %s", ast_get_name_of($1));
                }
            };

stmt_set : assign      { $$ = $1; trace("assign in stmt"); }
         | var_decl    { $$ = $1; trace("var_decl in stmt"); }
         | const_decl  { $$ = $1; trace("const_decl in stmt"); }
         | if_stmt     { $$ = $1; trace("if_stmt in stmt"); }
         | print_stmt  { $$ = $1; trace("print in stmt"); }
         | read_stmt   { $$ = $1; trace("read in stmt"); }
         | for_stmt    { $$ = $1; trace("for in stmt"); }
         | return_stmt { $$ = $1; trace("return in stmt"); }
         | invoke_stmt { $$ = $1; trace("invoke in stmt"); }
         ;

stmt : stmt_set           { $$ = $1; trace("stmt"); }
     | stmt_set SEMICOLON { $$ = $1; trace("stmt_with_semi"); };

stmts : stmt { $$ = $1; $$->next_stmt = NO_NODE; }
      | stmt stmts { $$ = $1; $1->next_stmt = $2; }
      ;

/* { ... code ... } */
begin_block : LEFT_BRACE { begin_context(); trace("begin of block"); };

end_block : RIGHT_BRACE { end_context(); trace("end of block"); };

block : begin_block stmts end_block { $$ = $2; };

/* special stmt */
print_stmt : PRINT expr { $$ = ast_create_node(PRINT_STMT); $$->flag = 0; $$->child = $2; }
           | PRINTLN expr { $$ = ast_create_node(PRINT_STMT); $$->flag = NEWLINE; $$->child = $2; }
           ;

read_stmt : READ id_eval { $$ = ast_create_node(READ_STMT); $$->child = $2; }
          ;

/* control flow */
if_stmt : IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS if_body {
            $$ = ast_create_if_node($3, $5, NO_NODE);
        }
        | IF LEFT_PARENTHESIS expr RIGHT_PARENTHESIS if_body ELSE if_body {
            $$ = ast_create_if_node($3, $5, $7);
        }
        ;

if_body : block { $$ = $1; }
        | stmt { $$ = $1; };

for_init_stmt : stmt_set { $$ = $1; $$->next_stmt = NO_NODE; }
              | { $$ = NO_NODE; }
              ;

for_cond : expr { $$ = $1; }
         ;

for_inc : stmt_set { $$ = $1; $$->next_stmt = NO_NODE; }
        | { $$ = NO_NODE; }
        ;

for_body : block { $$ = $1; }
         | stmt { $$ = $1; }
         ;

for_stmt : FOR LEFT_PARENTHESIS
               for_init_stmt SEMICOLON
               for_cond SEMICOLON
               for_inc
           RIGHT_PARENTHESIS for_body { $$ = ast_create_for_node($3, $5, $7, $9); }
         | FOR LEFT_PARENTHESIS
               for_cond SEMICOLON
               for_inc
           RIGHT_PARENTHESIS for_body { $$ = ast_create_for_node(NO_NODE, $3, $5, $7); }
         | FOR LEFT_PARENTHESIS
               for_cond
           RIGHT_PARENTHESIS for_body { $$ = ast_create_for_node(NO_NODE, $3, NO_NODE, $5); }
         ;

/* functions */

func_params_list : id_new type {
                     $$ = $1->meta = ast_create_var_node($2.t, PARAM, $2.n, $1, NO_NODE);
                     $$->next = NO_NODE;
                 }
                 | id_new type COMMA func_params_list {
                     $$ = $1->meta = ast_create_var_node($2.t, PARAM, $2.n, $1, NO_NODE);
                     $$->next = $4;
                 }
                 ;

func_params_def : func_params_list { $$ = $1; }
                | { $$ = NO_NODE; }
                ;

func_type_id :
             FUNC type id {
                trace("func type id");
                $$ = $3->meta = ast_create_func_node($3, $2.t, NO_NODE, NO_NODE);
                begin_context();
             }
             |
             FUNC id {
                trace("func type id");
                $$ = $2->meta = ast_create_func_node($2, VOID, NO_NODE, NO_NODE);
                begin_context();
             }
             ;

func_head :
          func_type_id
          LEFT_PARENTHESIS func_params_def RIGHT_PARENTHESIS {
              trace("begin of func");
              $$ = $1;
              $$->func.params = $3;
          };

func_decl :
          func_head block {
              begin_context();
              $$ = $1;
              $$->func.body = $2;
              // TODO: check return type
              trace("end of func");
              end_context();
              end_context();
          }
          ;

/* merge them into program */
/* TODO */
program : var_decl program   { $1->next_stmt = $2; $$ = prog = $1; }
        | const_decl program { $1->next_stmt = $2; $$ = prog = $1; }
        | func_decl program  { $1->next_stmt = $2; $$ = prog = $1; }
        | { $$ = NO_NODE; }
        ;


%%
void yyerror(const char *errmsg)
{
    fprintf(stderr, "PARSER_ERR: (%d, %d) %s\n", linenum, colnum, errmsg);
    exit(1);
}
/*
    vim: et
*/
