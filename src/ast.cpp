#include "ast.hpp"
#include <iostream>

using namespace std;

shared_ptr<NoProgram> root = nullptr;

No::~No() = default;
void No::print() { cout << "No::print()" << endl; }

void NoProgram::print() {
    cout << test << endl;
}

NoProgram::NoProgram(string test) {
    this->test = test;
}

NoProgram::~NoProgram() = default;
