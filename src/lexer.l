%{
enum {TOK_PROGRAM=256, TOK_BEGIN, TOK_END,
TOK_ASSIGNMENT, TOK_VAR, TOK_PROCEDURE, TOK_FUNCTION,
TOK_DOT, TOK_COLON, TOK_SEMICOLON, TOK_OPEN, TOK_CLOSE,
TOK_IF, TOK_THEN, TOK_ELSE,
TOK_WHILE, TOK_DO,
TOK_ADD, TOK_SUB, TOK_MUL, TOK_DIV,
TOK_E, TOK_NE, TOK_L, TOK_LE, TOK_G, TOK_GE,
TOK_OR, TOK_AND, TOK_NOT,
TOK_BOOLEAN, TOK_INTEGER,
TOK_READ, TOK_WRITE,
TOK_TRUE, TOK_FALSE,
TOK_NUMBER, TOK_ID, TOK_MALFORMED_NUM, TOK_ERROR};
char lexeme[100];
%}

%option yylineno

digit      [0-9]
id         [a-zA-Z][a-zA-Z0-9_]*
space      [ \t\n]

%%

"program"           { strcpy(lexeme, yytext); return TOK_PROGRAM; }
"begin"             { strcpy(lexeme, yytext); return TOK_BEGIN; }
"end"               { strcpy(lexeme, yytext); return TOK_END; }

":="                { strcpy(lexeme, yytext); return TOK_ASSIGNMENT; }
"var"               { strcpy(lexeme, yytext); return TOK_VAR; }
"procedure"         { strcpy(lexeme, yytext); return TOK_PROCEDURE; }
"function"          { strcpy(lexeme, yytext); return TOK_FUNCTION; }

"."                 { strcpy(lexeme, yytext); return TOK_DOT; }
":"                 { strcpy(lexeme, yytext); return TOK_COLON; }
";"                 { strcpy(lexeme, yytext); return TOK_SEMICOLON; }
"("                 { strcpy(lexeme, yytext); return TOK_OPEN; }
")"                 { strcpy(lexeme, yytext); return TOK_CLOSE; }

"if"                { strcpy(lexeme, yytext); return TOK_IF; } 
"then"              { strcpy(lexeme, yytext); return TOK_THEN; } 
"else"              { strcpy(lexeme, yytext); return TOK_ELSE; } 

"while"             { strcpy(lexeme, yytext); return TOK_WHILE; } 
"do"                { strcpy(lexeme, yytext); return TOK_DO; } 

"+"                 { strcpy(lexeme, yytext); return TOK_ADD; }
"-"                 { strcpy(lexeme, yytext); return TOK_SUB; }
"*"                 { strcpy(lexeme, yytext); return TOK_MUL; }
"div"               { strcpy(lexeme, yytext); return TOK_DIV; }

"="                 { strcpy(lexeme, yytext); return TOK_E; }
"<>"                { strcpy(lexeme, yytext); return TOK_NE; }
"<"                 { strcpy(lexeme, yytext); return TOK_L; }
"<="                { strcpy(lexeme, yytext); return TOK_LE; }
">"                 { strcpy(lexeme, yytext); return TOK_G; }
">="                { strcpy(lexeme, yytext); return TOK_GE; }

"or"                { strcpy(lexeme, yytext); return TOK_OR; }
"and"               { strcpy(lexeme, yytext); return TOK_AND; }
"not"               { strcpy(lexeme, yytext); return TOK_NOT; }

"boolean"           { strcpy(lexeme, yytext); return TOK_BOOLEAN; }
"integer"           { strcpy(lexeme, yytext); return TOK_INTEGER; } 

"read"              { strcpy(lexeme, yytext); return TOK_READ; } 
"write"             { strcpy(lexeme, yytext); return TOK_WRITE; }

"true"              { strcpy(lexeme, yytext); return TOK_TRUE; } 
"false"             { strcpy(lexeme, yytext); return TOK_FALSE; }

{digit}+{id}        { strcpy(lexeme, yytext); return TOK_MALFORMED_NUM; }
{digit}+            { strcpy(lexeme, yytext); return TOK_NUMBER; }
{id}                { strcpy(lexeme, yytext); return TOK_ID; }
{space}+            /* ignora espaços, tabs e quebras de linha */
.                   { strcpy(lexeme, yytext); return TOK_ERROR; }

%%

// Implementacao obrigatoria, pq eu tirei o %option noyywrap
int yywrap(void) {
    /* Retorna 1 para indicar "fim da entrada" */
    return 1;
}
