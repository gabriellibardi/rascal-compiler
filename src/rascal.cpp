#include "parser.tab.h"
#include <stdio.h>

int main (int argc, char** argv) {
   yyparse();
   printf("\n");
}
