%skeleton "lalr1.cc"
%require "3.7.4"

%language "c++"

%define api.value.type variant

%define api.token.constructor

%define parse.assert

%code requires {
    #include <vector>
    #include <memory>
    #include <string>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "ast.hpp"

    using namespace std;
}

%code {
    #include "grammar_aux.hpp"
}

%{
#include "ast.hpp"

using namespace std;
%}

%define api.token.prefix {TOK_}

%token TOK_PROGRAM TOK_BEGIN TOK_END TOK_ASSIGNMENT TOK_VAR
%token TOK_PROCEDURE TOK_FUNCTION TOK_DOT TOK_COMMA TOK_COLON TOK_SEMICOLON
%token TOK_OPEN TOK_CLOSE TOK_IF TOK_THEN TOK_ELSE TOK_WHILE TOK_DO
%token TOK_ADD TOK_SUB TOK_MUL TOK_DIV TOK_E TOK_NE TOK_L TOK_LE TOK_G TOK_GE
%token TOK_OR TOK_AND TOK_NOT TOK_READ TOK_WRITE
%token TOK_TRUE TOK_FALSE
%token TOK_ERROR TOK_MALFORMED_NUM

%token <int> TOK_NUMBER
%token <string> TOK_ID
%token <VarType> TOK_BOOLEAN TOK_INTEGER

%type <shared_ptr<NoProgram>> program block
%type <vector<shared_ptr<NoDeclaration>>> optional_var_declaration_section var_declaration_section
%type <shared_ptr<NoDeclaration>> var_declaration
%type <vector<string>> identifier_list
%type <VarType> type

%nonassoc IF_PREC
%nonassoc TOK_ELSE

%define parse.error verbose

%start program

%%

program                                
    : TOK_PROGRAM TOK_ID TOK_SEMICOLON block TOK_DOT {
        $$ = $4;
        root = $$;
    }
    ;

block                                  
    : optional_var_declaration_section subroutine_declaration_section composite_command {
        $$ = make_shared<NoProgram>($1);
    }
    ;

optional_var_declaration_section
    : var_declaration_section {
        $$ = $1;
    }
    | {
        $$ = vector<shared_ptr<NoDeclaration>>();
    }
    ;

var_declaration_section
    : TOK_VAR var_declaration TOK_SEMICOLON {
        $$ = {$2};
    }
    | var_declaration_section var_declaration TOK_SEMICOLON {
        $$ = $1;
        $$.push_back($2);
    }
    ;

var_declaration
    : identifier_list TOK_COLON type {
        $$ = make_shared<NoDeclaration>($1, $3);
    }
    ;

identifier_list
    : TOK_ID {
        $$ = {$1};
    }
    | identifier_list TOK_COMMA TOK_ID {
        $$ = $1;
        $$.push_back($3);
    }
    ;

type
    : TOK_BOOLEAN {
        $$ = $1;
    }
    | TOK_INTEGER {
        $$ = $1;
    }
    ;

subroutine_declaration_section
    :
    | subroutine_declaration_section procedure_declaration TOK_SEMICOLON
    | subroutine_declaration_section function_declaration TOK_SEMICOLON
    ;

procedure_declaration
    : TOK_PROCEDURE TOK_ID optional_formal_parameters TOK_SEMICOLON subroutine_block
    ;

function_declaration
    : TOK_FUNCTION TOK_ID optional_formal_parameters TOK_COLON type TOK_SEMICOLON subroutine_block
    ;

subroutine_block
    : optional_var_declaration_section composite_command
    ;


optional_formal_parameters
    : formal_parameters
    |
    ;

formal_parameters
    : TOK_OPEN parameter_declaration aux_formal_parameters TOK_CLOSE
    ;

aux_formal_parameters
    : aux_formal_parameters TOK_SEMICOLON parameter_declaration
    |
    ;

parameter_declaration
    : identifier_list TOK_COLON type
    ;

composite_command
    : TOK_BEGIN command aux_composite_command TOK_END
    ;

aux_composite_command
    : aux_composite_command TOK_SEMICOLON command
    |
    ;

command
    : assignment
    | procedure_call
    | conditional
    | repetition
    | read
    | write
    | composite_command
    ;

assignment
    : TOK_ID TOK_ASSIGNMENT expression
    ;

procedure_call
    : TOK_ID TOK_OPEN optional_expression_list TOK_CLOSE
    ;

optional_expression_list
    : expression_list
    |
    ;

conditional
    : TOK_IF expression TOK_THEN command %prec IF_PREC
    | TOK_IF expression TOK_THEN command TOK_ELSE command
    ;

repetition
    : TOK_WHILE expression TOK_DO command
    ;

read
    : TOK_READ TOK_OPEN identifier_list TOK_CLOSE
    ;

write
    : TOK_WRITE TOK_OPEN expression_list TOK_CLOSE
    ;

expression_list
    : expression aux_expression_list
    ;

aux_expression_list
    : aux_expression_list TOK_COMMA expression
    |
    ;

expression
    : simple_expression aux_expression
    ;

aux_expression
    : relation simple_expression
    |
    ;

relation
    : TOK_E
    | TOK_NE
    | TOK_L
    | TOK_LE
    | TOK_G
    | TOK_GE
    ;

simple_expression
    : term aux_simple_expression
    ;

aux_simple_expression
    : aux_simple_expression TOK_ADD term
    | aux_simple_expression TOK_SUB term
    | aux_simple_expression TOK_OR term
    |
    ;

term
    : factor aux_term
    ;

aux_term
    : aux_term TOK_MUL factor
    | aux_term TOK_DIV factor
    | aux_term TOK_AND factor
    |
    ;

factor
    : variable
    | TOK_NUMBER
    | logic
    | function_call
    | TOK_OPEN expression TOK_CLOSE
    | TOK_NOT factor
    | TOK_SUB factor
    ;

variable
    : TOK_ID
    ;

logic
    : TOK_FALSE
    | TOK_TRUE
    ;

function_call
    : TOK_ID TOK_OPEN optional_expression_list TOK_CLOSE
    ;

%%

namespace yy {
    void parser::error(const string& msg) {
        cout << "Parsing error: " << msg << endl;
    }
}
