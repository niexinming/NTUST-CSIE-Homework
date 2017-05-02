%{
#define LIST            strcat(buf, yytext)
#define token(t)               { LIST; token_pretty_print(t, (#t)+6); }
#define tokenInteger(t, i)     { LIST; printf("<int:%s>\n", i); }
#define tokenFloat(t, i)       { LIST; printf("<float:%s>\n", i); }
#define tokenString(t, s)      { LIST; token_dump_string(s); }
#define tokenIdentifier(t, id) { LIST; printf("<id:%s>\n", id); symtab_insert(id, symtab); }

#define dumpline { printf(" | %4d: %s", linenum++, buf); buf[0] = '\0'; }

#define MAX_LINE_LENG 65536

#include "tokens.h"
#include "symtab.h"

int linenum = 1;
char buf[MAX_LINE_LENG];
char string_buffer[MAX_LINE_LENG];

SYMTAB *symtab;
%}

%s SINGLE_LINE_COMMENT MULTI_LINE_COMMENTS IN_STRING IN_STRING_ESCAPED

digits		[0-9]+
identifier	[a-zA-Z_][a-zA-Z0-9_]*

%%

 /** delimiters **/

<INITIAL>","		{ token(TOKEN_COMMA); }
<INITIAL>":"		{ token(TOKEN_COLON); }
<INITIAL>";"		{ token(TOKEN_SEMICOLON); }

<INITIAL>"("		{ token(TOKEN_LEFT_PARENTHESIS); }
<INITIAL>")"		{ token(TOKEN_RIGHT_PARENTHESIS); }

<INITIAL>"["		{ token(TOKEN_LEFT_BRACKET); }
<INITIAL>"]"		{ token(TOKEN_RIGHT_BRACKET); }

<INITIAL>"{"		{ token(TOKEN_LEFT_BRACE); }
<INITIAL>"}"		{ token(TOKEN_RIGHT_BRACE); }


 /** unary operator **/

<INITIAL>"!"		{ token(TOKEN_LOGICAL_NOT); }
<INITIAL>"~"		{ token(TOKEN_BITWISE_NOT); }


 /** binary operator **/

<INITIAL>"+"		{ token(TOKEN_ADD); }
<INITIAL>"-"		{ token(TOKEN_SUB); }
<INITIAL>"*"		{ token(TOKEN_MUL); }
<INITIAL>"/"		{ token(TOKEN_DIV); }
<INITIAL>"%"		{ token(TOKEN_MOD); }
<INITIAL>"^"		{ token(TOKEN_XOR); }
<INITIAL>"&"		{ token(TOKEN_BITWISE_AND); }
<INITIAL>"|"		{ token(TOKEN_BITWISE_OR); }
<INITIAL>"&&"		{ token(TOKEN_LOGICAL_AND); }
<INITIAL>"||"		{ token(TOKEN_LOGICAL_OR); }


 /** compare **/

<INITIAL>"<"		{ token(TOKEN_LT); }
<INITIAL>">"		{ token(TOKEN_GT); }
<INITIAL>"<="		{ token(TOKEN_LTE); }
<INITIAL>">="		{ token(TOKEN_GTE); }
<INITIAL>"=="		{ token(TOKEN_EQ); }
<INITIAL>"!="		{ token(TOKEN_NEQ); }


 /** assignment **/

<INITIAL>"="		{ token(TOKEN_ASSIGN); }
<INITIAL>"+="		{ token(TOKEN_ASSIGN_ADD); }
<INITIAL>"-="		{ token(TOKEN_ASSIGN_SUB); }
<INITIAL>"*="		{ token(TOKEN_ASSIGN_MUL); }
<INITIAL>"/="		{ token(TOKEN_ASSIGN_DIV); }
<INITIAL>"%="		{ token(TOKEN_ASSIGN_MOD); }
<INITIAL>"^="		{ token(TOKEN_ASSIGN_XOR); }


 /** keywords **/

<INITIAL>"bool"		{ token(TOKEN_BOOL); }
<INITIAL>"break"	{ token(TOKEN_BREAK); }
<INITIAL>"case"		{ token(TOKEN_CASE); }
<INITIAL>"const"	{ token(TOKEN_CONST); }
<INITIAL>"continue"	{ token(TOKEN_CONTINUE); }
<INITIAL>"default"	{ token(TOKEN_DEFAULT); }
<INITIAL>"else"		{ token(TOKEN_ELSE); }
<INITIAL>"false"	{ token(TOKEN_FALSE); }
<INITIAL>"for"		{ token(TOKEN_FOR); }
<INITIAL>"func"		{ token(TOKEN_FUNC); }
<INITIAL>"go"		{ token(TOKEN_GO); }
<INITIAL>"if"		{ token(TOKEN_IF); }
<INITIAL>"import"	{ token(TOKEN_IMPORT); }
<INITIAL>"int"		{ token(TOKEN_INT); }
<INITIAL>"nil"		{ token(TOKEN_NIL); }
<INITIAL>"print"	{ token(TOKEN_PRINT); }
<INITIAL>"println"	{ token(TOKEN_PRINTLN); }
<INITIAL>"real"		{ token(TOKEN_REAL); }
<INITIAL>"return"	{ token(TOKEN_RETURN); }
<INITIAL>"string"	{ token(TOKEN_STRING); }
<INITIAL>"struct"	{ token(TOKEN_STRUCT); }
<INITIAL>"switch"	{ token(TOKEN_SWITCH); }
<INITIAL>"true"		{ token(TOKEN_TRUE); }
<INITIAL>"type"		{ token(TOKEN_TYPE); }
<INITIAL>"var"		{ token(TOKEN_VAR); }
<INITIAL>"void"		{ token(TOKEN_VOID); }
<INITIAL>"while"	{ token(TOKEN_WHILE); }


 /** identifier **/

<INITIAL>{identifier}	{ tokenIdentifier(TOKEN_ID, yytext); }


 /** single line comment **/

<INITIAL>"//" { LIST; BEGIN SINGLE_LINE_COMMENT; }
<SINGLE_LINE_COMMENT>.*\n { LIST; BEGIN INITIAL; dumpline; }


 /** multi-line comments **/

<INITIAL>"/*"				{ LIST; BEGIN MULTI_LINE_COMMENTS; }
<MULTI_LINE_COMMENTS>.		{ LIST; }
<MULTI_LINE_COMMENTS>\n		{ LIST; dumpline; }
<MULTI_LINE_COMMENTS>"*/"	{ LIST; BEGIN INITIAL; }


 /** string literal **/

<INITIAL>\"          { LIST; BEGIN IN_STRING; string_buffer[0] = 0; }
<IN_STRING>[^\"\\]   { LIST; strcat(string_buffer, yytext); }
<IN_STRING>\"        { tokenString(TOKEN_CONST_STRING, string_buffer); BEGIN INITIAL; }

<IN_STRING>"\\"      { LIST; BEGIN IN_STRING_ESCAPED; }
<IN_STRING_ESCAPED>n { LIST; strcat(string_buffer, "\n"); BEGIN IN_STRING; }
<IN_STRING_ESCAPED>r { LIST; strcat(string_buffer, "\r"); BEGIN IN_STRING; }
<IN_STRING_ESCAPED>0 { LIST; strcat(string_buffer, "\0"); BEGIN IN_STRING; }
<IN_STRING>\"\"      { LIST; strcat(string_buffer, "\""); }


 /** integer constant **/

<INITIAL>[+-]?[0-9]+ { tokenInteger(TOKEN_CONST_INT, yytext); }

 /** real number floating point constant **/

<INITIAL>[+-]?[0-9]*\.?[0-9]*(e-?[0-9]+)? { tokenFloat(TOKEN_CONST_FLOAT, yytext); }


 /** newline **/

\n      { LIST; dumpline; }


 /** spaces **/

[ \t]*  { LIST; }


 /** anything else is bad character **/
.	{
	LIST;
	printf("%d:%s\n", linenum+1, buf);
	printf("bad character:'%s'\n",yytext);
	exit(-1);
}

%%

int main()
{
	symtab = symtab_create();

	yylex();

	printf("\n"
			"  =======================\n"
			"  == symbol table dump ==\n"
			"  =======================\n"
			"\n");

	symtab_dump(symtab);
	symtab_destroy(symtab);
}
