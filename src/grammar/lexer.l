%{
#include "parser.tab.hpp"
#include "grammar_aux.hpp"
%}

%option noyywrap yylineno batch

digit      [0-9]
id         [a-zA-Z][a-zA-Z0-9_]*
space      [ \t\n]

%%

"program"           { return yy::parser::make_TOK_PROGRAM(); }
"begin"             { return yy::parser::make_TOK_BEGIN(); }
"end"               { return yy::parser::make_TOK_END(); }

":="                { return yy::parser::make_TOK_ASSIGNMENT(); }
"var"               { return yy::parser::make_TOK_VAR(); }
"procedure"         { return yy::parser::make_TOK_PROCEDURE(); }
"function"          { return yy::parser::make_TOK_FUNCTION(); }

"."                 { return yy::parser::make_TOK_DOT(); }
","                 { return yy::parser::make_TOK_COMMA();}
":"                 { return yy::parser::make_TOK_COLON(); }
";"                 { return yy::parser::make_TOK_SEMICOLON(); }
"("                 { return yy::parser::make_TOK_OPEN(); }
")"                 { return yy::parser::make_TOK_CLOSE(); }

"if"                { return yy::parser::make_TOK_IF(); } 
"then"              { return yy::parser::make_TOK_THEN(); } 
"else"              { return yy::parser::make_TOK_ELSE(); } 

"while"             { return yy::parser::make_TOK_WHILE(); } 
"do"                { return yy::parser::make_TOK_DO(); } 

"+"                 { return yy::parser::make_TOK_ADD(); }
"-"                 { return yy::parser::make_TOK_SUB(); }
"*"                 { return yy::parser::make_TOK_MUL(); }
"div"               { return yy::parser::make_TOK_DIV(); }

"="                 { return yy::parser::make_TOK_E(); }
"<>"                { return yy::parser::make_TOK_NE(); }
"<"                 { return yy::parser::make_TOK_L(); }
"<="                { return yy::parser::make_TOK_LE(); }
">"                 { return yy::parser::make_TOK_G(); }
">="                { return yy::parser::make_TOK_GE(); }

"or"                { return yy::parser::make_TOK_OR(); }
"and"               { return yy::parser::make_TOK_AND(); }
"not"               { return yy::parser::make_TOK_NOT(); }

"boolean"           { return yy::parser::make_TOK_BOOLEAN(); }
"integer"           { return yy::parser::make_TOK_INTEGER(); } 

"read"              { return yy::parser::make_TOK_READ(); } 
"write"             { return yy::parser::make_TOK_WRITE(); }

"true"              { return yy::parser::make_TOK_TRUE(); } 
"false"             { return yy::parser::make_TOK_FALSE(); }

{digit}+{id}        { return yy::parser::make_TOK_MALFORMED_NUM(); }
{digit}+            { return yy::parser::make_TOK_NUMBER(strtod(yytext, NULL)); }
{id}                { return yy::parser::make_TOK_ID(yytext); }
{space}+            /* ignora espaços, tabs e quebras de linha */
.                   { printf("Lexical error!\n"); return yy::parser::make_TOK_ERROR(); }

%%
