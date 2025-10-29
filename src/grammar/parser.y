%{
#include <stdio.h>
#include <string.h>

int yylex(void);
void yyerror(const char *);
%}

%union {
    char *sval;
    int  ival;
}

%token TOK_PROGRAM TOK_BEGIN TOK_END TOK_ASSIGNMENT TOK_VAR
%token TOK_PROCEDURE TOK_FUNCTION TOK_DOT TOK_COMMA TOK_COLON TOK_SEMICOLON
%token TOK_OPEN TOK_CLOSE TOK_IF TOK_THEN TOK_ELSE TOK_WHILE TOK_DO
%token TOK_ADD TOK_SUB TOK_MUL TOK_DIV TOK_E TOK_NE TOK_L TOK_LE TOK_G TOK_GE
%token TOK_OR TOK_AND TOK_NOT TOK_BOOLEAN TOK_INTEGER TOK_READ TOK_WRITE
%token TOK_TRUE TOK_FALSE
%token <ival> TOK_NUMBER
%token <sval> TOK_ID

%define parse.error verbose

%%

program                                : TOK_PROGRAM TOK_ID TOK_SEMICOLON block TOK_DOT
                                       ;

block                                  : optional_var_assignment_section optional_subroutine_assignment_section composite_command
                                       ;

optional_var_assignment_section        : var_assignment_section
                                       |
                                       ;

var_assignment_section                 : TOK_VAR var_assignment TOK_SEMICOLON
                                       | var_assignment_section var_assignment TOK_SEMICOLON
                                       ;

var_assignment                         : identifier_list TOK_COLON type
                                       ;

identifier_list                        : TOK_ID
                                       | identifier_list TOK_COMMA TOK_ID
                                       ;

type                                   : TOK_BOOLEAN
                                       | TOK_INTEGER
                                       ;

optional_subroutine_assignment_section : subroutine_assignment_section
                                       |
                                       ;

subroutine_assignment_section          :
                                       | subroutine_assignment_section procedure_declaration TOK_SEMICOLON
                                       | subroutine_assignment_section function_declaration TOK_SEMICOLON
                                       ;

procedure_declaration                  : TOK_PROCEDURE TOK_ID optional_formal_parameters TOK_SEMICOLON subroutine_block
                                       ;

function_declaration                   : TOK_FUNCTION TOK_ID optional_formal_parameters TOK_COLON type TOK_SEMICOLON subroutine_block
                                       ;

subroutine_block                       : optional_var_assignment_section composite_command
                                       ;


optional_formal_parameters             : formal_parameters
                                       |
                                       ;

formal_parameters                      : TOK_OPEN parameter_assignment aux_formal_parameters TOK_CLOSE
                                       ;

aux_formal_parameters                  : aux_formal_parameters TOK_SEMICOLON parameter_assignment
                                       |
                                       ;

parameter_assignment                   : identifier_list TOK_COLON type
                                       ;

composite_command                      : TOK_BEGIN command aux_composite_command TOK_END
                                       ;

aux_composite_command                  : aux_composite_command TOK_SEMICOLON command
                                       |
                                       ;

command                                : assignment
                                       | procedure_call
                                       | conditional
                                       | repetition
                                       | read
                                       | write
                                       | composite_command
                                       ;

assignment                             : TOK_ID TOK_ASSIGNMENT expression
                                       ;

procedure_call                         : TOK_ID TOK_OPEN optional_expression_list TOK_CLOSE
                                       ;

optional_expression_list               : expression_list
                                       |
                                       ;

conditional                            : TOK_IF expression TOK_THEN command optional_else_block
                                       ;

optional_else_block                    : TOK_ELSE command
                                       |
                                       ;

repetition                             : TOK_WHILE expression TOK_DO command
                                       ;

read                                   : TOK_READ TOK_OPEN identifier_list TOK_CLOSE
                                       ;

write                                  : TOK_WRITE TOK_OPEN expression_list TOK_CLOSE
                                       ;

expression_list                        : expression aux_expression_list
                                       ;

aux_expression_list                    : aux_expression_list TOK_COMMA expression
                                       |
                                       ;

expression                             : simple_expression aux_expression
                                       ;

aux_expression                         : relation simple_expression
                                       |
                                       ;

relation                               : TOK_E
                                       | TOK_NE
                                       | TOK_L
                                       | TOK_LE
                                       | TOK_G
                                       | TOK_GE
                                       ;

simple_expression                      : term aux_simple_expression
                                       ;

aux_simple_expression                  : aux_simple_expression TOK_ADD term
                                       | aux_simple_expression TOK_SUB term
                                       | aux_simple_expression TOK_OR term
                                       |
                                       ;

term                                   : factor aux_term
                                       ;

aux_term                               : aux_term TOK_MUL factor
                                       | aux_term TOK_DIV factor
                                       | aux_term TOK_AND factor
                                       |
                                       ;

factor                                 : variable
                                       | TOK_NUMBER
                                       | logic
                                       | function_call
                                       | TOK_OPEN expression TOK_CLOSE
                                       | TOK_NOT factor
                                       | TOK_SUB factor
                                       ;

variable                               : TOK_ID
                                       ;

logic                                  : TOK_FALSE
                                       | TOK_TRUE
                                       ;

function_call                          : TOK_ID TOK_OPEN optional_expression_list TOK_CLOSE
                                       ;

%%

void yyerror(const char * msg){
   fprintf(stderr, "ERROR: %s -> ", msg);
}
