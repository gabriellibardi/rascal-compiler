#include "ast.hpp"
#include <iostream>

using namespace std;

shared_ptr<NoProgram> root = nullptr;

No::~No() = default;
void No::print() { cout << "No::print()" << endl; }

void NoDeclaration::print() {
    cout << "(var";
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

void NoProgram::print() {
    cout << "(program";
    for (shared_ptr<NoDeclaration> decl: declaration_section) {
        cout << " ";
        decl->print();
    }
    cout << ")";
}

NoProgram::NoProgram(vector<shared_ptr<NoDeclaration>> decl_section) {
    this->declaration_section = decl_section;
}

NoProgram::~NoProgram() = default;
