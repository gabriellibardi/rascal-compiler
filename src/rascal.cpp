#include "rascal.hpp"
#include "ast.hpp"
#include "parser.tab.hpp"
#include "semantic/check_visitor.hpp"
#include "semantic/symbol_table.hpp"
#include "gen_visitor.hpp"

#include <iostream>
#include <fstream>
#include <memory>

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
   
   cout << "Running semantic analysis..." << endl;
   root->accept(&c_visitor);
   cout << "Semantic analysis finished!" << endl << endl;

   cout << "Generating MEPA code..." << endl;

   auto outfile = make_shared<ofstream>("build/out.mepa");
   if (!outfile->is_open()) {
      cerr << "Failed to open output file out.mepa" << endl;
      return 1;
   }

   GenVisitor g_visitor(symbol_manager, outfile);
   root->accept(&g_visitor);

   outfile->close();

   cout << "Code generation finished!" << endl;
   cout << "Output written to build/out.mepa" << endl;
   printf("\n");
   
   root->print();

   return 0;
}
