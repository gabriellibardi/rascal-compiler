#include "semantic/check_visitor.hpp"
#include <iostream>

using namespace std;

static void semantic_error(const string &msg) {
    cout << "semantic error! " << msg << endl;
}

void CheckVisitor::visit(NoDeclaration* no) {
    // Variáveis locais ou globais → address definido automaticamente no SymbolTableManager::install()
    for (string id : no->identifier_list) {
        auto s = make_shared<VarEntry>(id, no->var_type);
        if (!symbols->install(s)) {
            semantic_error(id + " redeclared");
        }
    }
}

void CheckVisitor::visit(NoSubroutine* no) {
    // Criar entrada global (sem escopo local ainda)
    shared_ptr<SymbolEntry> global_entry;

    if (no->rout_type == RoutType::FUNCTION) {
        global_entry = make_shared<FuncEntry>(
            no->identifier,
            vector<shared_ptr<ParamEntry>>{},
            no->return_type
        );
    } else {
        global_entry = make_shared<ProcEntry>(
            no->identifier,
            vector<shared_ptr<ParamEntry>>{}
        );
    }

    // Instalar no escopo GLOBAL
    if (!symbols->install(global_entry)) {
        semantic_error(no->identifier + " redeclared");
        return;
    }

    // Atribuir label
    if (global_entry->category == SymbolCategory::FUNCTION) {
        auto f = dynamic_pointer_cast<FuncEntry>(global_entry);
        f->label_num = next_label++;
    } else {
        auto p = dynamic_pointer_cast<ProcEntry>(global_entry);
        p->label_num = next_label++;
    }

    // Entrar no escopo local
    symbols->set_local(no->identifier);

    // ----------------------------
    // PARÂMETROS
    // ----------------------------
    vector<shared_ptr<ParamEntry>> param_entries;

    for (auto &param_decl : no->formal_parameters) {
        for (const string &id : param_decl->identifier_list) {
            auto p = make_shared<ParamEntry>(id, param_decl->var_type);

            // endereço automático via install()
            if (!symbols->install(p)) {
                semantic_error(id + " redeclared");
            } else {
                param_entries.push_back(p);
            }
        }
    }

    // ----------------------------
    // VARIÁVEIS LOCAIS
    // ----------------------------
    for (auto &var_decl : no->declaration_section) {
        for (const string &id : var_decl->identifier_list) {
            auto v = make_shared<VarEntry>(id, var_decl->var_type);

            if (!symbols->install(v)) {
                semantic_error(id + " redeclared");
            }
        }
    }

    // Atualizar info dos parâmetros na entrada global
    auto installed = symbols->search(no->identifier);
    if (installed) {
        if (installed->category == SymbolCategory::FUNCTION) {
            auto f = dynamic_pointer_cast<FuncEntry>(installed);
            f->param_list = param_entries;
        }
        else if (installed->category == SymbolCategory::PROCEDURE) {
            auto p = dynamic_pointer_cast<ProcEntry>(installed);
            p->param_list = param_entries;
        }
    }

    // Salvar ordem dos símbolos desta subrotina
    symbols->save_ordered_entries(no->identifier);

    // NÃO VOLTA PARA GLOBAL AQUI (isso é responsabilidade do GenVisitor)
}

void CheckVisitor::visit(NoUnaryExpr* no) {
    no->operand->accept(this);

    switch (no->op) {
        case Op::NOT:
            if (no->operand->inferred_type != VarType::BOOLEAN)
                semantic_error("operator 'not' requires boolean operand");
            no->inferred_type = VarType::BOOLEAN;
            break;

        case Op::SUB:
            if (no->operand->inferred_type != VarType::INTEGER)
                semantic_error("unary '-' requires integer operand");
            no->inferred_type = VarType::INTEGER;
            break;

        default:
            semantic_error("invalid unary operator");
            break;
    }
}

void CheckVisitor::visit(NoBinExpr* no) {
    no->left->accept(this);
    no->right->accept(this);

    VarType lt = no->left->inferred_type;
    VarType rt = no->right->inferred_type;

    switch (no->op) {
        case Op::ADD: case Op::SUB: case Op::MUL: case Op::DIV:
            if (lt != VarType::INTEGER || rt != VarType::INTEGER)
                semantic_error("arithmetic operators require integer operands");
            no->inferred_type = VarType::INTEGER;
            break;

        case Op::AND: case Op::OR:
            if (lt != VarType::BOOLEAN || rt != VarType::BOOLEAN)
                semantic_error("logical operators require boolean operands");
            no->inferred_type = VarType::BOOLEAN;
            break;

        case Op::L: case Op::LE: case Op::G: case Op::GE:
            if (lt != VarType::INTEGER || rt != VarType::INTEGER)
                semantic_error("relational operators require integer operands");
            no->inferred_type = VarType::BOOLEAN;
            break;

        case Op::E: case Op::NE:
            if (lt != rt)
                semantic_error("equality operators require operands of same type");
            no->inferred_type = VarType::BOOLEAN;
            break;

        default:
            semantic_error("invalid binary operator");
            break;
    }
}

void CheckVisitor::visit(NoVarExpr* no) {
    auto entry = symbols->search(no->identifier);

    if (!entry) {
        semantic_error("variable " + no->identifier + " not declared");
        no->inferred_type = VarType::INTEGER;
        return;
    }

    switch (entry->category) {
        case SymbolCategory::VARIABLE:
            no->inferred_type = dynamic_pointer_cast<VarEntry>(entry)->type;
            break;

        case SymbolCategory::PARAMETER:
            no->inferred_type = dynamic_pointer_cast<ParamEntry>(entry)->type;
            break;

        case SymbolCategory::FUNCTION:
            semantic_error("identifier " + no->identifier + " is a function, use call");
            no->inferred_type = dynamic_pointer_cast<FuncEntry>(entry)->return_type;
            break;

        default:
            semantic_error("identifier " + no->identifier + " is not a variable");
            no->inferred_type = VarType::INTEGER;
            break;
    }
}

void CheckVisitor::visit(NoLiteralExpr* no) {
    no->inferred_type = no->type;
}

void CheckVisitor::visit(NoCallExpr* no) {
    auto entry = symbols->search(no->identifier);

    if (!entry || entry->category != SymbolCategory::FUNCTION) {
        semantic_error(no->identifier + " is not a function");
        no->inferred_type = VarType::INTEGER;
        return;
    }

    auto f = dynamic_pointer_cast<FuncEntry>(entry);

    if (f->param_list.size() != no->expression_list.size())
        semantic_error("argument count mismatch in call to " + no->identifier);

    for (size_t i = 0; i < no->expression_list.size(); ++i) {
        no->expression_list[i]->accept(this);
        if (i < f->param_list.size() &&
            no->expression_list[i]->inferred_type != f->param_list[i]->type) {
            semantic_error("argument type mismatch in call to " + no->identifier);
        }
    }

    no->inferred_type = f->return_type;
}

void CheckVisitor::visit(NoCompositeCommand* no) {
    for (auto &cmd : no->command_list)
        cmd->accept(this);
}

void CheckVisitor::visit(NoAssignment* no) {
    auto entry = symbols->search(no->identifier);

    if (!entry) {
        semantic_error("variable " + no->identifier + " not declared");
        no->expr->accept(this);
        return;
    }

    no->expr->accept(this);

    VarType target;

    if (entry->category == SymbolCategory::VARIABLE)
        target = dynamic_pointer_cast<VarEntry>(entry)->type;
    else if (entry->category == SymbolCategory::PARAMETER)
        target = dynamic_pointer_cast<ParamEntry>(entry)->type;
    else if (entry->category == SymbolCategory::FUNCTION)
        target = dynamic_pointer_cast<FuncEntry>(entry)->return_type;
    else {
        semantic_error("cannot assign to " + no->identifier);
        return;
    }

    if (no->expr->inferred_type != target)
        semantic_error("type mismatch in assignment to " + no->identifier);
}

void CheckVisitor::visit(NoProcedureCall* no) {
    auto entry = symbols->search(no->identifier);

    if (!entry || entry->category != SymbolCategory::PROCEDURE) {
        semantic_error(no->identifier + " is not a procedure");
        return;
    }

    auto p = dynamic_pointer_cast<ProcEntry>(entry);

    if (p->param_list.size() != no->expression_list.size())
        semantic_error("argument count mismatch in call to " + no->identifier);

    for (size_t i = 0; i < no->expression_list.size(); ++i) {
        no->expression_list[i]->accept(this);
        if (i < p->param_list.size() &&
            no->expression_list[i]->inferred_type != p->param_list[i]->type) {
            semantic_error("argument type mismatch in call to " + no->identifier);
        }
    }
}

void CheckVisitor::visit(NoConditional* no) {
    no->condition->accept(this);
    if (no->condition->inferred_type != VarType::BOOLEAN)
        semantic_error("condition in 'if' must be boolean");
    if (no->then_cmd) no->then_cmd->accept(this);
    if (no->else_cmd) no->else_cmd->accept(this);
}

void CheckVisitor::visit(NoRepetition* no) {
    no->condition->accept(this);
    if (no->condition->inferred_type != VarType::BOOLEAN)
        semantic_error("condition in 'while' must be boolean");
    if (no->body) no->body->accept(this);
}

void CheckVisitor::visit(NoRead* no) {
    for (auto &id : no->identifier_list) {
        auto entry = symbols->search(id);
        if (!entry) {
            semantic_error("variable " + id + " not declared");
            continue;
        }
        if (entry->category != SymbolCategory::VARIABLE &&
            entry->category != SymbolCategory::PARAMETER) {
            semantic_error("read expects a variable: " + id);
        }
    }
}

void CheckVisitor::visit(NoWrite* no) {
    for (auto &expr : no->expression_list)
        expr->accept(this);
}

void CheckVisitor::visit(NoProgram* no) {
    auto prog = make_shared<ProgramEntry>(no->identifier);

    if (!symbols->install(prog))
        semantic_error("program " + no->identifier + " redeclared");

    for (auto &decl : no->declaration_section)
        decl->accept(this);

    for (auto &r : no->subroutine_section)
        r->accept(this);

    if (no->body)
        no->body->accept(this);
}

CheckVisitor::CheckVisitor(shared_ptr<SymbolTableManager> symbols) {
    this->symbols = symbols;
}
