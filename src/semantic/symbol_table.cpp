#include "symbol_table.hpp"

SymbolEntry::SymbolEntry(
    string name, SymbolCategory category
) : name(name), category(category) { }

VarEntry::VarEntry(
    string name, VarType type
) : SymbolEntry(name, SymbolCategory::VARIABLE), type(type) { }

ParamEntry::ParamEntry(
    string name, VarType type
) : SymbolEntry(name, SymbolCategory::PARAMETER), type(type) { }

ProcEntry::ProcEntry(
    string name, vector<shared_ptr<ParamEntry>> param_list
) : SymbolEntry(name, SymbolCategory::PROCEDURE), param_list(param_list) { }

FuncEntry::FuncEntry(
    string name, vector<shared_ptr<ParamEntry>> param_list, VarType return_type
) : SymbolEntry(name, SymbolCategory::FUNCTION), param_list(param_list), return_type(return_type) { }

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

SymbolTableManager::SymbolTableManager() {
    this->state = Scope::GLOBAL;
    this->global_table = make_shared<SymbolTable>();
    this->active_table = this->global_table;
    this->rout_tables = map<string, shared_ptr<SymbolTable>>();
    this->ordered_active_entries = vector<shared_ptr<SymbolEntry>>();
}
