#include "ast.hpp"
#include <iostream>

using namespace std;

shared_ptr<NoProgram> root = nullptr;

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

NoProgram::NoProgram(vector<shared_ptr<NoDeclaration>> decl_section, vector<shared_ptr<NoSubroutine>> rout_section) {
    this->declaration_section = decl_section;
    this->subroutine_section = rout_section;
}

NoProgram::~NoProgram() = default;
