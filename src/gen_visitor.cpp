#include "gen_visitor.hpp"
#include <iostream>
#include <fstream>

using namespace std;

int GenVisitor::new_label() {
    return label_count++;
}

void GenVisitor::emit_label(int label_num) {
    (*out_file) << "R" << (label_num < 10 ? "0" + to_string(label_num) : to_string(label_num)) << ": NADA" << endl;
}

void GenVisitor::emit(const string &code) {
    (*out_file) << "     " << code << endl;
}

void GenVisitor::emit(const string &code, int a) {
    (*out_file) << "     " << code << " " << a << endl;
}

void GenVisitor::emit(const string &code, int a, int b) {
    (*out_file) << "     " << code << " " << a << "," << b << endl;
}

void GenVisitor::emit_label_ref(const string &code, int label_num) {
    string lab = (label_num < 10 ? "R0" + to_string(label_num) : "R" + to_string(label_num));
    (*out_file) << "     " << code << " " << lab << endl;
}

void GenVisitor::emit_label_ref(const string &code, int label_num, int level) {
    string lab = (label_num < 10 ? "R0" + to_string(label_num) : "R" + to_string(label_num));
    (*out_file) << "     " << code << " " << lab << "," << level << endl;
}

bool GenVisitor::load_var_info(const string &id, int &level, int &address) {
    auto entry = symbols->search(id);
    if (!entry) {
        cerr << "CG ERROR: undefined id: " << id << endl;
        return false;
    }

    if (entry->category == SymbolCategory::VARIABLE) {
        auto var = dynamic_pointer_cast<VarEntry>(entry);
        level = (var->scope == Scope::GLOBAL ? 0 : 1);
        address = var->address;
        return true;
    }

    if (entry->category == SymbolCategory::PARAMETER) {
        auto p = dynamic_pointer_cast<ParamEntry>(entry);
        level = 1;
        address = p->address;
        return true;
    }

    if (entry->category == SymbolCategory::FUNCTION) {
        auto func = dynamic_pointer_cast<FuncEntry>(entry);
        level = 1;
        address = -5 - func->param_list.size(); 
        return true;
    }

    return false;
}

void GenVisitor::visit(NoDeclaration* no) {
    if (symbols->state == Scope::LOCAL) {
        int total = no->identifier_list.size();
        if (total > 0) emit("AMEM", total);
    }
}

void GenVisitor::visit(NoSubroutine* no) {
    auto entry = symbols->search(no->identifier);
    if (!entry) {
        cerr << "CG ERROR: unknown subroutine '" << no->identifier << "'" << endl;
        return;
    }

    int label = -1;
    int param_count = 0;

    if (entry->category == SymbolCategory::PROCEDURE) {
        auto proc = dynamic_pointer_cast<ProcEntry>(entry);
        label = proc->label_num;
        param_count = proc->param_list.size();
    }
    else if (entry->category == SymbolCategory::FUNCTION) {
        auto func = dynamic_pointer_cast<FuncEntry>(entry);
        label = func->label_num;
        param_count = func->param_list.size();
    }
    else {
        cerr << "CG ERROR: subroutine '" << no->identifier
             << "' is not procedure/function" << endl;
        return;
    }

    emit_label(label);

    emit("ENPR " + to_string(param_count));

    auto locals = symbols->get_ordered_active_entries(no->identifier);
    
    int local_count = 0;
    for (auto &sym : locals) {
        if (sym->category == SymbolCategory::VARIABLE)
            local_count++;
    }

    if (local_count > 0)
        emit("AMEM", local_count);

    if (no->body)
        no->body->accept(this);

    if (local_count > 0)
        emit("DMEM", local_count);

    emit("RTPR " + to_string(param_count));
}

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
    int level, address;
    if (!load_var_info(no->identifier, level, address))
        return;

    emit("CRVL", level, address);
}

void GenVisitor::visit(NoLiteralExpr* no) {
    if (no->type == VarType::INTEGER)
        emit("CRCT", no->num);
    else
        emit("CRCT", no->logic ? 1 : 0);
}

void GenVisitor::visit(NoCallExpr* no) {
    auto entry = symbols->search(no->identifier);

    if (!entry || entry->category != SymbolCategory::FUNCTION) {
        cerr << "CG ERROR: '" << no->identifier << "' is not a function" << endl;
        return;
    }

    auto func = dynamic_pointer_cast<FuncEntry>(entry);
    
    emit("AMEM 1");

    for (auto &expr : no->expression_list)
        expr->accept(this);

    int level = (func->scope == Scope::GLOBAL ? 0 : 1);

    emit_label_ref("CHPR", func->label_num, level);
}

void GenVisitor::visit(NoCompositeCommand* no) {
    for (auto &cmd : no->command_list)
        cmd->accept(this);
}

void GenVisitor::visit(NoAssignment* no) {
    int level, address;
    if (!load_var_info(no->identifier, level, address))
        return;

    no->expr->accept(this);

    emit("ARMZ", level, address);
}

void GenVisitor::visit(NoProcedureCall* no) {
    auto entry = symbols->search(no->identifier);

    if (!entry || entry->category != SymbolCategory::PROCEDURE) {
        cerr << "CG ERROR: '" << no->identifier << "' is not a procedure" << endl;
        return;
    }

    auto proc = dynamic_pointer_cast<ProcEntry>(entry);

    for (auto &expr : no->expression_list)
        expr->accept(this);

    int level = (proc->scope == Scope::GLOBAL ? 0 : 1);

    emit_label_ref("CHPR", proc->label_num, level);
}

void GenVisitor::visit(NoConditional* no) {
    int Lfalse = new_label();
    int Lend = new_label();

    no->condition->accept(this);
    emit_label_ref("DSVF", Lfalse);

    no->then_cmd->accept(this);
    emit_label_ref("DSVS", Lend);

    emit_label(Lfalse);
    if (no->else_cmd)
        no->else_cmd->accept(this);

    emit_label(Lend);
}

void GenVisitor::visit(NoRepetition* no) {
    int Lstart = new_label();
    int Lexit = new_label();

    emit_label(Lstart);

    no->condition->accept(this);
    emit_label_ref("DSVF", Lexit);

    no->body->accept(this);
    emit_label_ref("DSVS", Lstart);

    emit_label(Lexit);
}

void GenVisitor::visit(NoRead* no) {
    for (auto &id : no->identifier_list) {
        int level, address;
        if (!load_var_info(id, level, address))
            continue;

        emit("LEIT");
        emit("ARMZ", level, address);
    }
}

void GenVisitor::visit(NoWrite* no) {
    for (auto &expr : no->expression_list) {
        expr->accept(this);
        emit("IMPR");
    }
}

void GenVisitor::visit(NoProgram* no) {
    emit("INPP");

    int total_globals = 0;
    for (auto &decl : no->declaration_section)
        total_globals += decl->identifier_list.size();

    if (total_globals > 0)
        emit("AMEM", total_globals);

    bool has_subroutines = !no->subroutine_section.empty();

    int main_label = -1;

    if (has_subroutines) {
        main_label = new_label();
        emit_label_ref("DSVS", main_label);
    }

    for (auto &rout : no->subroutine_section)
        rout->accept(this);

    if (has_subroutines) {
        emit_label(main_label);
    }
    if (no->body)
        no->body->accept(this);

    if (total_globals > 0)
        emit("DMEM", total_globals);

    emit("PARA");
    emit("FIM");
}

GenVisitor::GenVisitor(shared_ptr<SymbolTableManager> symbols, shared_ptr<ofstream> out_file, int initial_label) {
    this->label_count = initial_label;
    this->symbols = symbols;
    this->out_file = out_file;
}
