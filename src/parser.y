%{
#include <stdio.h>
#include <string.h>

int yylex(void);
void yyerror(const char *);
%}

%token TOK_PROGRAM 

%define parse.error verbose

%%

program       : TOK_PROGRAM TOK_ID TOK_SEMICOLON block TOK_DOT
              ;

block         : optional_var_assignment_section optional_subroutine_assignment_section composite_command
              ;

optional_var_assignment_section  : var_assignment_section
                                 |
                                 ;

var_assignment_section  : TOK_VAR var_assignment TOK_SEMICOLON
                        | var_assignment_section var_assignment TOK_SEMICOLON
                        ;

var_assignment          : identifier_list TOK_COLON type
                        ;

identifier_list         : TOK_ID
                        | identifier_list TOK_COMMA TOK_ID
                        ;

type                    : TOK_BOOLEAN
                        | TOK_INTEGER
                        ;

optional_subroutine_assignment_section : subroutine_assignment_section
                                       |
                                       ;

subroutine_assignment_section :
                              | subroutine_assignment_section procedure_declaration TOK_SEMICOLON
                              | subroutine_assignment_section function_declaration TOK_SEMICOLON
                              ;

procedure_declaration         : TOK_PROCEDURE TOK_ID optional_formal_parameters TOK_SEMICOLON subroutine_block
                              ;

function_declaration          : TOK_FUNCTION TOK_ID optional_formal_parameters TOK_COLON type TOK_SEMICOLON subroutine_block
                              ;

subroutine_block              : optional_var_assignment_section composite_command
                              ;


optional_formal_parameters    : formal_parameters
                              |
                              ;

formal_parameters             : TOK_OPEN parameter_assignment aux_formal_parameters TOK_CLOSE
                              ;

aux_formal_parameters         : aux_formal_parameters TOK_SEMICOLON parameter_assignment
                              |
                              ;

parameter_assignment          : identifier_list TOK_COLON type
                              ;

composite_command             : TOK_BEGIN command aux_composite_command TOK_END
                              ;

aux_composite_command         : assinment
                              | proceure_call
                              | conditional
                              | repetition
                              | read
                              | write
                              | composite_command
                              ;

%%

void yyerror(const char * msg){
   fprintf(stderr, "ERROR: %s -> ", msg);
}
