#pragma once

#include <vector>
#include <memory>
#include <string>

using namespace std;

enum class VarType { INTEGER, BOOLEAN };
enum class RoutType { PROCEDURE, FUNCTION };

class No {
    public:
        virtual ~No();
        virtual void print();
};

class NoDeclaration : public No {
    public:
        vector<string> identifier_list;
        VarType var_type;

        void print() override;

        NoDeclaration(vector<string> id_list, VarType var_type);
        ~NoDeclaration();
};

class NoSubroutine : public No {
    public:
        string identifier;
        RoutType rout_type;
        VarType return_type;
        vector<shared_ptr<NoDeclaration>> formal_parameters;
        vector<shared_ptr<NoDeclaration>> declaration_section;

        void print() override;

        NoSubroutine(string identifier, RoutType rout_type, VarType return_type, vector<shared_ptr<NoDeclaration>> formal_param, vector<shared_ptr<NoDeclaration>> decl_section);
        ~NoSubroutine();
};

class NoProgram : public No {
    public:
        string identifier;
        vector<shared_ptr<NoDeclaration>> declaration_section;
        vector<shared_ptr<NoSubroutine>> subroutine_section;

        void print() override;

        NoProgram(vector<shared_ptr<NoDeclaration>> decl_section, vector<shared_ptr<NoSubroutine>> rout_section);
        ~NoProgram();
};

extern shared_ptr<NoProgram> root;
