#include <map>
#include <vector>
#include <memory>
#include <string>

#include "ast.hpp"

using namespace std;

enum class SymbolCategory { VARIABLE, PARAMETER, PROCEDURE, FUNCTION};
enum class Scope { GLOBAL, LOCAL };

class SymbolEntry {
    public:
        string name;
        SymbolCategory category;
        Scope scope;

        SymbolEntry(string name, SymbolCategory category, Scope scope);
        virtual ~SymbolEntry();
};

class VarEntry : public SymbolEntry {
    public:
        VarType type;
        int address;

        VarEntry(
            string name, Scope scope,
            VarType type
        );
        ~VarEntry() = default;
};

class ParamEntry : public SymbolEntry {
    public:
        VarType type;
        int address;

        ParamEntry(
            string name, Scope scope,
            VarType type
        );
        ~ParamEntry() = default;
};

class ProcEntry : public SymbolEntry {
    public:
        vector<ParamEntry> param_list;
        string label;
        
        ProcEntry(
            string name, Scope scope,
            vector<ParamEntry> param_list
        );
        ~ProcEntry() = default;
};

class FuncEntry : public SymbolEntry {
    public:
        vector<ParamEntry> param_list;
        VarType return_type;
        string label;

        FuncEntry(
            string name, Scope scope,
            vector<ParamEntry> param_list, VarType return_type
        );
        ~FuncEntry() = default;
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
        Scope state;
        shared_ptr<SymbolTable> active_table, global_table;
        map<string, shared_ptr<SymbolTable>> rout_tables;
    public:
        bool install(shared_ptr<SymbolEntry> symbol);
        shared_ptr<SymbolEntry> search(string identifier);
        bool set_local(string rout_id);
        void set_global();

        SymbolTableManager();
        ~SymbolTableManager() = default;
};
