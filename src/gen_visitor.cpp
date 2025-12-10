#include "gen_visitor.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int GenVisitor::new_label() {
    return label_count++;
}

void GenVisitor::emit_label(int label_num) {
    (*out_file) << "R" << label_num << ": NADA" << endl;
}

void GenVisitor::emit(const string &code) {
    (*out_file) << '\t' << code << endl;
}

void GenVisitor::emit(const string &code, int a) {
    (*out_file) << '\t' << code << " " << a << endl;
}

void GenVisitor::emit(const string &code, int a, int b) {
    (*out_file) << '\t' << code << " " << a << "," << b << endl;
}

void GenVisitor::visit(NoDeclaration* no) { }
void GenVisitor::visit(NoSubroutine* no) { }
void GenVisitor::visit(NoUnaryExpr* no) { }
void GenVisitor::visit(NoBinExpr* no) { }

void GenVisitor::visit(NoVarExpr* no) {
    
}

void GenVisitor::visit(NoLiteralExpr* no) { }
void GenVisitor::visit(NoCallExpr* no) { }
void GenVisitor::visit(NoCompositeCommand* no) { }
void GenVisitor::visit(NoAssignment* no) { }
void GenVisitor::visit(NoProcedureCall* no) { }
void GenVisitor::visit(NoConditional* no) { }
void GenVisitor::visit(NoRepetition* no) { }
void GenVisitor::visit(NoRead* no) { }
void GenVisitor::visit(NoWrite* no) { }

void GenVisitor::visit(NoProgram* no) {
    emit("INPP");

    for (auto &decl : no->declaration_section)
        decl->accept(this);

    for (auto &rout : no->subroutine_section)
        rout->accept(this);

    no->body->accept(this);

    emit("PARA");
}

GenVisitor::GenVisitor(shared_ptr<SymbolTableManager> symbols, shared_ptr<ofstream> out_file) {
    this->label_count = 0;
    this->symbols = symbols;
    this->out_file = out_file;
}
