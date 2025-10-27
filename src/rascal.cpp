#include "parser.tab.h"
#include <stdio.h>
#include "rascal.hpp"

int main (int argc, char** argv) {
   yyparse();
   printf("\n");
}
