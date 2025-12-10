#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>

#include "ast.hpp"

using namespace std;

enum class SymbolCategory { VARIABLE, PARAMETER, PROCEDURE, FUNCTION, PROGRAM};
enum class Scope { GLOBAL, LOCAL };

class SymbolEntry {
    public:
        string name;
        SymbolCategory category;
        Scope scope;

        SymbolEntry(string name, SymbolCategory category);
        virtual ~SymbolEntry() = default;
};

class VarEntry : public SymbolEntry {
    public:
        VarType type;
        int address;

        VarEntry(string name, VarType type);
        ~VarEntry() = default;
};

class ParamEntry : public SymbolEntry {
    public:
        VarType type;
        int address;

        ParamEntry(string name, VarType type);
        ~ParamEntry() = default;
};

class ProcEntry : public SymbolEntry {
    public:
        vector<shared_ptr<ParamEntry>> param_list;
        int label_num;
        
        ProcEntry(string name, vector<shared_ptr<ParamEntry>> param_list);
        ~ProcEntry() = default;
};

class FuncEntry : public SymbolEntry {
    public:
        vector<shared_ptr<ParamEntry>> param_list;
        VarType return_type;
        int label_num;

        FuncEntry(string name, vector<shared_ptr<ParamEntry>> param_list, VarType return_type);
        ~FuncEntry() = default;
};

class ProgramEntry : public SymbolEntry {
    public:
        ProgramEntry(string name);
        ~ProgramEntry() = default;
};

class SymbolTable {
    private:
        map<string, shared_ptr<SymbolEntry>> symbols;
    public:
        bool install(shared_ptr<SymbolEntry> symbol);
        shared_ptr<SymbolEntry> search(string identifier);

        SymbolTable();
        ~SymbolTable() = default;
};

class SymbolTableManager {
    private:
        shared_ptr<SymbolTable> active_table, global_table;
        map<string, shared_ptr<SymbolTable>> rout_tables;
        vector<shared_ptr<SymbolEntry>> ordered_active_entries;
    public:
        Scope state;
        
        bool install(shared_ptr<SymbolEntry> symbol);
        shared_ptr<SymbolEntry> search(string identifier);
        bool set_local(string rout_id);
        void set_global();
        vector<shared_ptr<SymbolEntry>> get_ordered_active_entries();

        SymbolTableManager();
        ~SymbolTableManager() = default;
};
