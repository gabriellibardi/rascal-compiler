#include "rascal.hpp"
#include "ast.hpp"
#include "parser.tab.hpp"
#include <stdio.h>

using namespace std;

int main (int argc, char** argv) {
   yy::parser p;
   int success = p.parse();
   if (root == nullptr) {
      cout << "AST root is null!" << endl;
   } else if (success == 0) {
      root->print();
   } else {
      cout << "parsing insuccessfull" << endl;
   }
   printf("\n");
}
