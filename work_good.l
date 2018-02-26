%{

#include <stdio.h>
#include "work_good.tab.h"

%}

number 0|[1-9][0-9]*
integerNumber {number}|\-{number}
floatNumber {integerNumber}\.[0-9]+

%%



"const"                                { return CONST; }
"void"                                 { return VOID_TYPE; }
"int"|"float"|"char"|"string"          { yylval.StringValue = strdup(yytext); return VAR_TYPE; }



"print"                                { return PRINT; }
"return"                               { return RETURN; }


"for"                                   { return FOR; }
"while"                                 { return WHILE; }
"if"                                    { return IF; }
"else"                                  { return ELSE; }



"true"                                  { return TRUE; }
"false"                                 { return FALSE; }



">"|"<"|">="|"<="|"=="|"!="|"&&"|"||"   { return BOOL_OP; }



"struct"                                { return STRUCT; }



"+=" 					{ return ASSIGN_PLUS; }
"-=" 					{ return ASSIGN_MINUS; }
"*=" 					{ return ASSIGN_PROD; }
"/=" 					{ return ASSIGN_DIV; }
"%=" 					{ return ASSIGN_MOD; }
"="                                     { return ASSIGN;}



"-"                                     { return MINUS; }
"+"                                     { return PLUS; }
"/"                                     { return DIVISION; }
"*"                                     { return MULTIPLY; }
"%"                                     { return MODULO; }



{integerNumber}                         { yylval.intNumberValue = atoi(yytext); return INTEGER; }
{floatNumber}                           { yylval.floatNumberValue = atof (yytext); return FLOAT; }
\'.\' 					{ return CHAR; }
\".*\" 					{ return STRING; }



[_a-zA-Z][_a-zA-Z0-9]* 			{ yylval.StringValue = strdup(yytext); return ID; }




[ \t] ;
\n 															        { yylineno++; }
. 														        	{ return yytext[0]; }
<<EOF>> 													      { return TEOF; }


%%
