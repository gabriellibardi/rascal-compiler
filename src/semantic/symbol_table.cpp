#include "symbol_table.hpp"

SymbolEntry::SymbolEntry(
    string name, SymbolCategory category, Scope scope
) : name(name), category(category), scope(scope) { }

VarEntry::VarEntry(
    string name, Scope scope,
    VarType type
) : SymbolEntry(name, SymbolCategory::VARIABLE, scope), type(type) { }

ParamEntry::ParamEntry(
    string name, Scope scope,
    VarType type
) : SymbolEntry(name, SymbolCategory::PARAMETER, scope), type(type) { }

ProcEntry::ProcEntry(
    string name, Scope scope,
    vector<ParamEntry> param_list
) : SymbolEntry(name, SymbolCategory::PROCEDURE, scope), param_list(param_list) { }

FuncEntry::FuncEntry(
    string name, Scope scope,
    vector<ParamEntry> param_list, VarType return_type
) : SymbolEntry(name, SymbolCategory::FUNCTION, scope), param_list(param_list), return_type(return_type) { }

bool SymbolTable::install(shared_ptr<SymbolEntry> symbol) {

}

shared_ptr<SymbolEntry> SymbolTable::search(string identifier) {

}

SymbolTable::SymbolTable() {
    this->symbols = map<string, shared_ptr<SymbolEntry>>();
}

bool SymbolTableManager::install(shared_ptr<SymbolEntry> symbol) {

}

shared_ptr<SymbolEntry> SymbolTableManager::search(string identifier) {

}

void SymbolTableManager::set_global() { }

bool SymbolTableManager::set_local(string rout_id) { }
