#pragma once

#include <vector>
#include <memory>
#include <string>
#include <fstream>

#include "ast.hpp"
#include "semantic/symbol_table.hpp"

using namespace std;

class GenVisitor : public Visitor {
    private:
        shared_ptr<ofstream> out_file;
        int label_count;
        
        int new_label();
        void emit(const string &code);
        void emit(const string &code, int a);
        void emit(const string &code, int a, int b);
        void emit_label(int label_num);
        void emit_label_ref(const string &code, int label_num);
        void emit_label_ref(const string &code, int label_num, int level);
        bool load_var_info(const string &id, int &level, int &address);
    public:
        void visit(NoDeclaration* no) override;
        void visit(NoSubroutine* no) override;
        void visit(NoUnaryExpr* no) override;
        void visit(NoBinExpr* no) override;
        void visit(NoVarExpr* no) override;
        void visit(NoLiteralExpr* no) override;
        void visit(NoCallExpr* no) override;
        void visit(NoCompositeCommand* no) override;
        void visit(NoAssignment* no) override;
        void visit(NoProcedureCall* no) override;
        void visit(NoConditional* no) override;
        void visit(NoRepetition* no) override;
        void visit(NoRead* no) override;
        void visit(NoWrite* no) override;
        void visit(NoProgram* no) override;

        GenVisitor(shared_ptr<SymbolTableManager> symbols, shared_ptr<ofstream> out_file, int initial_label);
        ~GenVisitor() = default;
};
