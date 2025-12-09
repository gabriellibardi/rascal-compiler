#include "check_visitor.hpp"
#include <iostream>

using namespace std;

void CheckVisitor::visit(NoDeclaration* no) {
    for (string id : no->identifier_list) {
        shared_ptr<SymbolEntry> s = make_shared<VarEntry>(id, no->var_type);
        if (!this->symbols->install(s)) {
            cout << "semantic error! " << id << " redeclared" << endl;
        }
    }
}

void CheckVisitor::visit(NoSubroutine* no) {
    if (!this->symbols->set_local(no->identifier)) {
        cout << "semantic error! " << no->identifier << " redeclared" << endl;
        return;
    }
    for (auto param : no->formal_parameters) {
        param->accept(this);
    }
    auto param_entries = vector<shared_ptr<ParamEntry>>();
    for (shared_ptr<SymbolEntry> entry : this->symbols->get_ordered_active_entries()) {
        if (entry->category == SymbolCategory::PARAMETER) {
            param_entries.push_back(dynamic_pointer_cast<ParamEntry>(entry));
        }
    }
    for (auto var : no->declaration_section) {
        var->accept(this);
    }
    shared_ptr<SymbolEntry> s; 
    switch (no->rout_type) {
    case RoutType::FUNCTION:
        s = make_shared<FuncEntry>(no->identifier, param_entries, no->return_type);
        break;
    case RoutType::PROCEDURE:
        s = make_shared<ProcEntry>(no->identifier, param_entries);
        break;
    }
    if (!this->symbols->install(s)) {
        cout << "error at semantic analysis of " << no->identifier << " subroutine declaration" << endl;
    }
}

CheckVisitor::CheckVisitor(shared_ptr<SymbolTableManager> symbols) {
    this->symbols = symbols;
}
