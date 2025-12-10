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

bool GenVisitor::load_var_info(const string &id, int &level, int &address) {
    auto entry = symbols->search(id);
    if (!entry) {
        cerr << "CG ERROR: undefined id: " << id << endl;
        return false;
    }

    if (entry->category == SymbolCategory::VARIABLE) {
        auto var = dynamic_pointer_cast<VarEntry>(entry);
        level = var->scope == Scope::GLOBAL ? 0 : 1;
        address = var->address;
        return true;
    }

    if (entry->category == SymbolCategory::PARAMETER) {
        auto p = dynamic_pointer_cast<ParamEntry>(entry);
        level = 1;
        address = p->address;
        return true;
    }

    return false;
}

void GenVisitor::visit(NoDeclaration* no) { }
void GenVisitor::visit(NoSubroutine* no) { }

void GenVisitor::visit(NoUnaryExpr* no) {
    no->operand->accept(this);

    switch (no->op) {
        case Op::SUB:
            emit("INVR");
            break;

        case Op::NOT:
            emit("NEGA");
            break;

        default:
            cerr << "Unsupported unary op in codegen" << endl;
            break;
    }
}

void GenVisitor::visit(NoBinExpr* no) {
    no->left->accept(this);
    no->right->accept(this);

    switch (no->op) {
        case Op::ADD: emit("SOMA"); break;
        case Op::SUB: emit("SUBT"); break;
        case Op::MUL: emit("MULT"); break;
        case Op::DIV: emit("DIVI"); break;

        case Op::AND: emit("CONJ"); break;
        case Op::OR:  emit("DISJ"); break;

        case Op::E:   emit("CMIG"); break;
        case Op::NE:  emit("CMDG"); break;
        case Op::L:   emit("CMME"); break;
        case Op::LE:  emit("CMEG"); break;
        case Op::G:   emit("CMMA"); break;
        case Op::GE:  emit("CMAG"); break;   

        default:
            cerr << "Unsupported binary op in codegen" << endl;
            break;
    }
}

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

void GenVisitor::visit(NoWrite* no) {
    for (auto &expr : no->expression_list) {
        expr->accept(this);
        emit("IMPR");
    }
}

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
