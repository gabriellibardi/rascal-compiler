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
    s->scope = state;

    if (!active_table->install(s))
        return false;

    if (state == Scope::GLOBAL) {
        if (s->category == SymbolCategory::VARIABLE) {
            auto v = dynamic_pointer_cast<VarEntry>(s);
            v->address = global_offset++;
        }
    } 
    else if (state == Scope::LOCAL) {
        if (s->category == SymbolCategory::PARAMETER) {
            auto p = dynamic_pointer_cast<ParamEntry>(s);
            p->address = param_offset--;
        }
        else if (s->category == SymbolCategory::VARIABLE) {
            auto v = dynamic_pointer_cast<VarEntry>(s);
            v->address = local_offset++;
        }

        ordered_active_entries.push_back(s);
    }

    return true;
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
    state = Scope::GLOBAL;
    active_table = global_table;
    ordered_active_entries.clear();
}

bool SymbolTableManager::set_local(string rout_id) {
    state = Scope::LOCAL;

    if (rout_tables.count(rout_id) == 1) {
        active_table = rout_tables[rout_id];
    } else {
        auto new_table = make_shared<SymbolTable>();
        rout_tables[rout_id] = new_table;
        active_table = new_table;
    }

    ordered_active_entries.clear();

    local_offset = 0;
    param_offset = -5;

    return true;
}

vector<shared_ptr<SymbolEntry>> SymbolTableManager::get_ordered_active_entries() {
    return ordered_active_entries;
}

vector<shared_ptr<SymbolEntry>> SymbolTableManager::get_ordered_active_entries(const string &rout_id) {
    if (this->rout_ordered_entries.count(rout_id) == 0) return vector<shared_ptr<SymbolEntry>>();
    return this->rout_ordered_entries[rout_id];
}

void SymbolTableManager::save_ordered_entries(const string &rout_id) {
    this->rout_ordered_entries[rout_id] = this->ordered_active_entries;
    this->ordered_active_entries = vector<shared_ptr<SymbolEntry>>();
}

SymbolTableManager::SymbolTableManager() {
    state = Scope::GLOBAL;
    global_table = make_shared<SymbolTable>();
    active_table = global_table;

    rout_tables = {};
    ordered_active_entries = {};

    global_offset = 0;
    local_offset = 0;
    param_offset = -5;
}
