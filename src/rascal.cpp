#include "rascal.hpp"
#include "ast.hpp"
#include "parser.tab.hpp"
#include "semantic/check_visitor.hpp"
#include "semantic/symbol_table.hpp"

#include <iostream>

using namespace std;

int main (int argc, char** argv) {
   yy::parser p;

   int success = p.parse();

   if (success != 0) {
      cout << "Parsing unsuccessful" << endl;
      return 1;
   }

   if (root == nullptr) {
      cout << "AST root is null!" << endl;
      return 1;
   }

   cout << "Parsing succeeded!" << endl;
   printf("\n");

   auto symbol_manager = make_shared<SymbolTableManager>();
   CheckVisitor c_visitor(symbol_manager);
   root->accept(&c_visitor);
   cout << "Semantic analysis finished!" << endl;

   root->print();

   return 0;
}
