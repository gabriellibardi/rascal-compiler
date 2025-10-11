%{
#include <stdio.h>
#include <string.h>

int yylex(void);
void yyerror(const char *);
%}

%token TOK_PROGRAM 

%define parse.error verbose

%%

program       : TOK_PROGRAM
              ;

%%

void yyerror(const char * msg){
   fprintf(stderr, "ERROR: %s -> ", msg);
}
