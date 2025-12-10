#include "semantic/symbol_table.hpp"
#include <iostream>

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

ProgramEntry::ProgramEntry(string name)
: SymbolEntry(name, SymbolCategory::PROGRAM) { }

bool SymbolTable::install(shared_ptr<SymbolEntry> s) {
    if (this->symbols.count(s->name) == 1) {
        return false;
    }
    this->symbols[s->name] = s;
    return true;
}

shared_ptr<SymbolEntry> SymbolTable::search(string identifier) {
    if (this->symbols.count(identifier) == 0) {
        return nullptr;
    }
    return this->symbols[identifier];
}

SymbolTable::SymbolTable() {
    this->symbols = map<string, shared_ptr<SymbolEntry>>();
}

bool SymbolTableManager::install(shared_ptr<SymbolEntry> s) {
    s->scope = this->state;
    return this->active_table->install(s);
}

shared_ptr<SymbolEntry> SymbolTableManager::search(string identifier) {
    shared_ptr<SymbolEntry> local_result;
    switch (this->state) {
    case Scope::GLOBAL:
        return this->active_table->search(identifier);
        break;
    case Scope::LOCAL:
        local_result = this->active_table->search(identifier);
        if (local_result != nullptr) return local_result;
        return this->global_table->search(identifier);
        break;
    default:
        cout << "scope type value that is neither local nor global at SymbolTableManager::search()" << endl;
        return nullptr;
        break;
    }
}

void SymbolTableManager::set_global() {
    this->state = Scope::GLOBAL;
    this->active_table = this->global_table;
    this->ordered_active_entries = vector<shared_ptr<SymbolEntry>>();
}

bool SymbolTableManager::set_local(string rout_id) {
    if (this->rout_tables.count(rout_id) == 1) {
        return false;
    }
    this->state = Scope::LOCAL;
    auto new_table = make_shared<SymbolTable>();
    this->rout_tables[rout_id] = new_table;
    this->active_table = new_table;
    this->ordered_active_entries = vector<shared_ptr<SymbolEntry>>();
    return true;
}

SymbolTableManager::SymbolTableManager() {
    this->state = Scope::GLOBAL;
    this->global_table = make_shared<SymbolTable>();
    this->active_table = this->global_table;
    this->rout_tables = map<string, shared_ptr<SymbolTable>>();
    this->ordered_active_entries = vector<shared_ptr<SymbolEntry>>();
}
