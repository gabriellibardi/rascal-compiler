%{
#include <stdio.h>
#include <string.h>

int yylex(void);
void yyerror(const char *);
%}

%token TOK_PROGRAM 

%define parse.error verbose

%%

assignment                 : TOK_ID TOK_ASSIGNMENT expression
                           ;

procedure_call             : TOK_ID TOK_OPEN optional_expression_list TOK_CLOSE
                           ;

optional_expression_list   : expression_list
                           |
                           ;

conditional                : TOK_IF expression TOK_THEN command optional_else_block
                           ;

optional_else_block:       : TOK_ELSE command
                           |
                           ;

repetition                 : TOK_WHILE expression TOK_DO command
                           ;

read                       : TOK_READ TOK_OPEN identifier_list TOK_CLOSE
                           ;

writ                       : TOK_WRITE TOK_OPEN expression_list TOK_CLOSE
                           ;

expression_list            : expression aux_expression_list
                           ;

aux_expression_list        : aux_expression_list TOK_COMMA expression
                           |
                           ;

expression                 : simple_expression aux_expression
                           ;

aux_expression             : relation simple_expression
                           |
                           ;

relation                   : TOK_E
                           | TOK_NE
                           | TOK_L
                           | TOK_LE
                           | TOK_G
                           | TOK_GE
                           ;

simple_expression          : term aux_simple_expression
                           ;

aux_simple_expression      : aux_simple_expression TOK_ADD term
                           : aux_simple_expression TOK_SUB term
                           : aux_simple_expression TOK_OR term
                           |
                           ;

term                       : factor aux_term
                           ;

aux_term                   : aux_term TOK_MUL factor
                           | aux_term TOK_DIV factor
                           | aux_term TOK_AND factor
                           |
                           ;

factor                     : variable
                           | number
                           | logic
                           | function_call
                           | TOK_OPEN expression TOK_CLOSE
                           | TOK_NOT factor
                           | TOK_SUB factor
                           ;

variable                   : identifier
                           ;

logic                      : TOK_FALSE
                           | TOK_TRUE
                           ;

function_call              : identifier TOK_OPEN optional_expression_list TOK_CLOSE
                           ;

%%

void yyerror(const char * msg){
   fprintf(stderr, "ERROR: %s -> ", msg);
}
