#include "ast.hpp"
#include <iostream>

using namespace std;

shared_ptr<NoProgram> root = nullptr;

string op_string(Op op) {
    switch (op) {
    case Op::E:
        return "=";
        break;
    case Op::NE:
        return "<>";
        break;
    case Op::L:
        return "<";
        break;
    case Op::LE:
        return "<=";
        break;
    case Op::G:
        return ">";
        break;
    case Op::GE:
        return ">=";
        break;
    case Op::ADD:
        return "+";
        break;
    case Op::SUB:
        return "-";
        break;
    case Op::MUL:
        return "*";
        break;
    case Op::DIV:
        return "div";
        break;
    case Op::OR:
        return "or";
        break;
    case Op::AND:
        return "and";
        break;
    case Op::NOT:
        return "not";
        break;
    }
}

No::~No() = default;
void No::print() { cout << "No::print()" << endl; }

void NoDeclaration::print() {
    cout << "(";
    for (string id: identifier_list) {
        cout << " " << id;
    }
    cout << " : ";
    switch (var_type) {
    case VarType::INTEGER:
        cout << "integer)";
        break;
    case VarType::BOOLEAN:
        cout << "boolean)";
        break;
    }
}

NoDeclaration::NoDeclaration(vector<string> id_list, VarType var_type) {
    this->identifier_list = id_list;
    this->var_type = var_type;
}

NoDeclaration::~NoDeclaration() = default;

void NoSubroutine::print() {
    switch (rout_type) {
    case RoutType::PROCEDURE:
        cout << "(procedure";
        break;
    case RoutType::FUNCTION:
        cout << "(function";
        break;
    }
    cout << " " << identifier;
    for (shared_ptr<NoDeclaration> param : formal_parameters) {
        cout << " ";
        param->print();
    }
    cout << " (var";
    for (shared_ptr<NoDeclaration> decl : declaration_section) {
        cout << " ";
        decl->print();
    }
    cout << ")";
    if (rout_type == RoutType::FUNCTION) {
        switch (return_type) {
        case VarType::INTEGER:
            cout << " <- integer";
            break;
        case VarType::BOOLEAN:
            cout << " <- boolean";
            break;
        }
    }
    cout << ")";
}

NoSubroutine::NoSubroutine(string identifier, RoutType rout_type, VarType return_type, vector<shared_ptr<NoDeclaration>> formal_param, vector<shared_ptr<NoDeclaration>> decl_section) {
    this->identifier = identifier;
    this->rout_type = rout_type;
    this->return_type = return_type;
    this->formal_parameters = formal_param;
    this->declaration_section = decl_section;
}

NoSubroutine::~NoSubroutine() = default;

void NoUnaryExpr::print() {
    cout << "(" << op_string(op) << " ";
    operand->print();
    cout << ")";
}

NoUnaryExpr::NoUnaryExpr(shared_ptr<NoExpression> operand, Op op) {
    this->operand = operand;
    this->op = op;
}

void NoBinExpr::print() {
    cout << "(" << op_string(op) << " ";
    left->print();
    cout << " ";
    right->print();
    cout << ")";
}

NoBinExpr::NoBinExpr(shared_ptr<NoExpression> left, shared_ptr<NoExpression> right, Op op) {
    this->left = left;
    this->right = right;
    this->op = op;
}

void NoVarExpr::print() {
    cout << identifier;
}

NoVarExpr::NoVarExpr(string identifier) {
    this->identifier = identifier;
}

void NoLiteralExpr::print() {
    switch (type) {
    case VarType::INTEGER:
        cout << num;
        break;
    case VarType::BOOLEAN:
        cout << logic;
        break;
    }
}

NoLiteralExpr::NoLiteralExpr(int num) {
    this->num = num;
    this->type = VarType::INTEGER;
}

NoLiteralExpr::NoLiteralExpr(bool logic) {
    this->logic = logic;
    this->type = VarType::BOOLEAN; 
}

void NoCallExpr::print() {
    cout << "(call " << identifier;
    for (shared_ptr<NoExpression> expr: expression_list) {
        cout << " ";
        expr->print();
    }
    cout << ")";
}

NoCallExpr::NoCallExpr(string identifier, vector<shared_ptr<NoExpression>> expression_list) {
    this->identifier = identifier;
    this->expression_list = expression_list;
}

NoCompositeCommand::NoCompositeCommand(vector<shared_ptr<NoCommand>> cmds) {
    this->command_list = cmds;
}

void NoCompositeCommand::print() {
    cout << "(begin";
    for (auto &cmd : command_list) {
        cout << " ";
        cmd->print();
    }
    cout << " end)";
}

NoAssignment::NoAssignment(string id, shared_ptr<NoExpression> expr) {
    this->identifier = id;
    this->expr = expr;
}

void NoAssignment::print() {
    cout << "(" << identifier << " := ";
    expr->print();
    cout << ")";
}

NoProcedureCall::NoProcedureCall(string id) {
    this->identifier = id;
}

NoProcedureCall::NoProcedureCall(string id, vector<shared_ptr<NoExpression>> exprs) {
    this->identifier = id;
    this->expression_list = exprs;
}

void NoProcedureCall::print() {
    cout << "(call " << identifier;
    for (auto &expr : expression_list) {
        cout << " ";
        expr->print();
    }
    cout << ")";
}

NoConditional::NoConditional(shared_ptr<NoExpression> cond,
                             shared_ptr<NoCommand> then_cmd,
                             shared_ptr<NoCommand> else_cmd) {
    this->condition = cond;
    this->then_cmd = then_cmd;
    this->else_cmd = else_cmd;
}

void NoConditional::print() {
    cout << "(if ";
    condition->print();
    cout << " then ";
    then_cmd->print();
    if (else_cmd) {
        cout << " else ";
        else_cmd->print();
    }
    cout << ")";
}

NoRepetition::NoRepetition(shared_ptr<NoExpression> cond,
                           shared_ptr<NoCommand> body) {
    this->condition = cond;
    this->body = body;
}

void NoRepetition::print() {
    cout << "(while ";
    condition->print();
    cout << " do ";
    body->print();
    cout << ")";
}

NoRead::NoRead(vector<string> list) {
    this->identifier_list = list;
}

void NoRead::print() {
    cout << "(read";
    for (auto &id : identifier_list) {
        cout << " " << id;
    }
    cout << ")";
}

NoWrite::NoWrite(vector<shared_ptr<NoExpression>> list) {
    this->expression_list = list;
}

void NoWrite::print() {
    cout << "(write";
    for (auto &expr : expression_list) {
        cout << " ";
        expr->print();
    }
    cout << ")";
}


void NoProgram::print() {
    cout << "(program " << identifier << " ";
    cout << "(var";
    for (shared_ptr<NoDeclaration> decl: declaration_section) {
        cout << " ";
        decl->print();
    }
    cout << ")";
    for (shared_ptr<NoSubroutine> rout: subroutine_section) {
        cout << " ";
        rout->print();
    }
    cout << ")";
}

NoProgram::NoProgram(vector<shared_ptr<NoDeclaration>> decl_section, vector<shared_ptr<NoSubroutine>> rout_section, shared_ptr<NoCommand> body) {
    this->declaration_section = decl_section;
    this->subroutine_section = rout_section;
    this->body = body;
}

NoProgram::~NoProgram() = default;
