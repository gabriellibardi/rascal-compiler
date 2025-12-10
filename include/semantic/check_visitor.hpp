#pragma once

#include <vector>
#include <memory>
#include <string>

#include "ast.hpp"
#include "symbol_table.hpp"

using namespace std;

class CheckVisitor : public Visitor {
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

        CheckVisitor(shared_ptr<SymbolTableManager> symbols);
        ~CheckVisitor() = default;
};
