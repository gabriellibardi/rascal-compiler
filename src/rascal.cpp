#include "rascal.hpp"
#include "ast.hpp"
#include "parser.tab.hpp"
#include <iostream>

using namespace std;

int main (int argc, char** argv) {
   yy::parser p;
   int success = p.parse();
   if (root == nullptr) {
      cout << "AST root is null!" << endl;
   } else if (success == 0) {
      cout << "parsing succeded" << endl; 
      root->print();
   } else {
      cout << "parsing unsuccessfull" << endl;
   }
   printf("\n");
}
