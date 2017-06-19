%{
#define DETAIL_LOG 0

#if DETAIL_LOG == 1
#define PRINTF(FMT, ...) printf(FMT, __VA_ARGS__)
#else
#define PRINTF(FMT, ...)
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "strdup.h"

#define LIST                   { strcat(buf, yytext); colnum += strlen(yytext); }
#define token(t)               { LIST; if(DETAIL_LOG) token_pretty_print(t, (#t)); return t; }
#define tokenInteger(t, i)     { LIST; PRINTF("<int:%s>\n", i); yylval.integer = atoi(i); return t; }
#define tokenReal(t, i)        { LIST; sscanf(i, "%lf", &yylval.real); PRINTF("<real:%g>\n", yylval.real); return t; }
#define tokenString(t, s)      { LIST; if(DETAIL_LOG) token_dump_string(s); yylval.string = s; return t; }
#define tokenIdentifier(t, id) { LIST; PRINTF("<id:%s>\n", id); yylval.string = mystrdup(id); return t; }

#define dumpline { \
                 buf[colnum-1] = 0; \
				 if(getenv("DUMPLINE") || scanner_dumpline) printf(" | %4d: %s ", linenum, buf); \
				 linenum++; colnum = 0; buf[0] = '\0'; }

#define MAX_LINE_LENG 65536

#include "tokens.h"

int scanner_dumpline = 0;

int colnum = 1;
int linenum = 1;
char buf[MAX_LINE_LENG];
char string_buffer[MAX_LINE_LENG];
%}

%s SINGLE_LINE_COMMENT MULTI_LINE_COMMENTS IN_STRING IN_STRING_ESCAPED

digits		[0-9]+
identifier	[a-zA-Z_][a-zA-Z0-9_]*

%%

 /** delimiters **/

<INITIAL>","		{ token(COMMA); }
<INITIAL>":"		{ token(COLON); }
<INITIAL>";"		{ token(SEMICOLON); }

<INITIAL>"("		{ token(LEFT_PARENTHESIS); }
<INITIAL>")"		{ token(RIGHT_PARENTHESIS); }

<INITIAL>"["		{ token(LEFT_BRACKET); }
<INITIAL>"]"		{ token(RIGHT_BRACKET); }

<INITIAL>"{"		{ token(LEFT_BRACE); }
<INITIAL>"}"		{ token(RIGHT_BRACE); }


 /** unary operator **/

<INITIAL>"!"		{ token(LOGICAL_NOT); }
<INITIAL>"~"		{ token(BITWISE_NOT); }


 /** binary operator **/

<INITIAL>"+"		{ token(ADD); }
<INITIAL>"-"		{ token(SUB); }
<INITIAL>"*"		{ token(MUL); }
<INITIAL>"/"		{ token(DIV); }
<INITIAL>"%"		{ token(MOD); }
<INITIAL>"^"		{ token(XOR); }
<INITIAL>"&"		{ token(BITWISE_AND); }
<INITIAL>"|"		{ token(BITWISE_OR); }
<INITIAL>"&&"		{ token(LOGICAL_AND); }
<INITIAL>"||"		{ token(LOGICAL_OR); }


 /** compare **/

<INITIAL>"<"		{ token(LT); }
<INITIAL>">"		{ token(GT); }
<INITIAL>"<="		{ token(LTE); }
<INITIAL>">="		{ token(GTE); }
<INITIAL>"=="		{ token(EQ); }
<INITIAL>"!="		{ token(NEQ); }


 /** assignment **/

<INITIAL>"="		{ token(ASSIGN); }
<INITIAL>"+="		{ token(ASSIGN_ADD); }
<INITIAL>"-="		{ token(ASSIGN_SUB); }
<INITIAL>"*="		{ token(ASSIGN_MUL); }
<INITIAL>"/="		{ token(ASSIGN_DIV); }
<INITIAL>"%="		{ token(ASSIGN_MOD); }
<INITIAL>"^="		{ token(ASSIGN_XOR); }


 /** keywords **/

<INITIAL>"bool"		{ token(BOOL); }
<INITIAL>"break"	{ token(BREAK); }
<INITIAL>"case"		{ token(CASE); }
<INITIAL>"const"	{ token(CONST); }
<INITIAL>"continue"	{ token(CONTINUE); }
<INITIAL>"default"	{ token(DEFAULT); }
<INITIAL>"else"		{ token(ELSE); }
<INITIAL>"false"	{ token(FALSE); }
<INITIAL>"for"		{ token(FOR); }
<INITIAL>"func"		{ token(FUNC); }
<INITIAL>"go"		{ token(GO); }
<INITIAL>"if"		{ token(IF); }
<INITIAL>"import"	{ token(IMPORT); }
<INITIAL>"int"		{ token(INT); }
<INITIAL>"nil"		{ token(NIL); }
<INITIAL>"print"	{ token(PRINT); }
<INITIAL>"println"	{ token(PRINTLN); }
<INITIAL>"real"		{ token(REAL); }
<INITIAL>"return"	{ token(RETURN); }
<INITIAL>"string"	{ token(STRING); }
<INITIAL>"struct"	{ token(STRUCT); }
<INITIAL>"switch"	{ token(SWITCH); }
<INITIAL>"true"		{ token(TRUE); }
<INITIAL>"type"		{ token(TYPE); }
<INITIAL>"var"		{ token(VAR); }
<INITIAL>"void"		{ token(VOID); }
<INITIAL>"while"	{ token(WHILE); }
<INITIAL>"read"     { token(READ); }


 /** identifier **/

<INITIAL>{identifier}	{ tokenIdentifier(ID, yytext); }


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
<IN_STRING>[^\\"]    { LIST; strcat(string_buffer, yytext); }
<IN_STRING>\"        { BEGIN INITIAL; tokenString(CONST_STRING, string_buffer); }

<IN_STRING>"\\"      { LIST; BEGIN IN_STRING_ESCAPED; }
<IN_STRING_ESCAPED>n { LIST; BEGIN IN_STRING; strcat(string_buffer, "\n"); }
<IN_STRING_ESCAPED>r { LIST; BEGIN IN_STRING; strcat(string_buffer, "\r"); }
<IN_STRING_ESCAPED>0 { LIST; BEGIN IN_STRING; strcat(string_buffer, "\0"); }
<IN_STRING>\"\"      { LIST; strcat(string_buffer, "\""); }


 /** integer constant **/

<INITIAL>[+-]?[0-9]+ { tokenInteger(CONST_INT, yytext); }

 /** real number floating point constant **/

<INITIAL>[+-]?[0-9]*\.?[0-9]*(e-?[0-9]+)? { tokenReal(CONST_REAL, yytext); }


 /** newline **/

\n      { LIST; dumpline; colnum = 1; }


 /** spaces **/

[ \t]*  { LIST; }


 /** anything else is bad character **/
.	{
	LIST;
	printf("%d:%s\n", linenum+1, buf);
	printf("bad character:'%s'\n",yytext);
	exit(-1);
}
