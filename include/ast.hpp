#pragma once

#include <any>
#include <map>
#include <vector>
#include <memory>
#include <string>

using namespace std;

enum class VarType { INTEGER, BOOLEAN };
enum class RoutType { PROCEDURE, FUNCTION };
enum class Op {
    E, NE, L, LE, G, GE,
    ADD, SUB, MUL, DIV,
    OR, AND, NOT
};

using AuxMap = map<string, any>;

string op_string(Op op);

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

class NoExpression : public No {
    public:
        VarType inferred_type;

        ~NoExpression() = default;
};

class NoUnaryExpr : public NoExpression {
    public:
        shared_ptr<NoExpression> operand;
        Op op;

        void print() override;

        NoUnaryExpr(shared_ptr<NoExpression> operand, Op op);
        ~NoUnaryExpr() = default;
};

class NoBinExpr : public NoExpression {
    public:
        shared_ptr<NoExpression> left, right;
        Op op;

        void print() override;

        NoBinExpr(shared_ptr<NoExpression> left, shared_ptr<NoExpression> right, Op op);
        ~NoBinExpr() = default;
};

class NoVarExpr : public NoExpression {
    public:
        string identifier;

        void print() override;

        NoVarExpr(string identifier);
        ~NoVarExpr() = default;
};

class NoLiteralExpr : public NoExpression {
    public:
        int num;
        bool logic;
        VarType type;

        void print() override;

        NoLiteralExpr(int num);
        NoLiteralExpr(bool logic);
        ~NoLiteralExpr() = default;
};

class NoCallExpr : public NoExpression {
    public:
        string identifier;
        vector<shared_ptr<NoExpression>> expression_list;

        void print() override;

        NoCallExpr(string identifier, vector<shared_ptr<NoExpression>> expression_list);
        ~NoCallExpr() = default;
};

class NoCommand : public No {
    public:
        virtual ~NoCommand() = default;
};

class NoCompositeCommand : public NoCommand {
    public:
        vector<shared_ptr<NoCommand>> command_list;

        NoCompositeCommand(vector<shared_ptr<NoCommand>> cmds);
        
        void print() override;
};

class NoAssignment : public NoCommand {
    public:
        string identifier;
        shared_ptr<NoExpression> expr;

        NoAssignment(string id, shared_ptr<NoExpression> expr);
        
        void print() override;
};

class NoProcedureCall : public NoCommand {
    public:
        string identifier;
        vector<shared_ptr<NoExpression>> expression_list;

        NoProcedureCall(string id);
        NoProcedureCall(string id, vector<shared_ptr<NoExpression>> exprs);
    
        void print() override;
};

class NoConditional : public NoCommand {
    public:
        shared_ptr<NoExpression> condition;
        shared_ptr<NoCommand> then_cmd;
        shared_ptr<NoCommand> else_cmd;

        NoConditional(shared_ptr<NoExpression> cond,
                      shared_ptr<NoCommand> then_cmd,
                      shared_ptr<NoCommand> else_cmd = nullptr);
        
        void print() override;
};

class NoRepetition : public NoCommand {
    public:
        shared_ptr<NoExpression> condition;
        shared_ptr<NoCommand> body;

        NoRepetition(shared_ptr<NoExpression> cond,
                     shared_ptr<NoCommand> body);
    
        void print() override;
};

class NoRead : public NoCommand {
    public:
        vector<string> identifier_list;

        NoRead(vector<string> list);
        
        void print() override;
};

class NoWrite : public NoCommand {
    public:
        vector<shared_ptr<NoExpression>> expression_list;

        NoWrite(vector<shared_ptr<NoExpression>> list);
    
        void print() override;
};

class NoCommand : public No {
    public:
        virtual ~NoCommand() = default;
};

class NoCompositeCommand : public NoCommand {
    public:
        vector<shared_ptr<NoCommand>> command_list;

        NoCompositeCommand(vector<shared_ptr<NoCommand>> cmds);
        
        void print() override;
};

class NoAssignment : public NoCommand {
    public:
        string identifier;
        shared_ptr<NoExpression> expr;

        NoAssignment(string id, shared_ptr<NoExpression> expr);
        
        void print() override;
};

class NoProcedureCall : public NoCommand {
    public:
        string identifier;
        vector<shared_ptr<NoExpression>> expression_list;

        NoProcedureCall(string id);
        NoProcedureCall(string id, vector<shared_ptr<NoExpression>> exprs);
    
        void print() override;
};

class NoConditional : public NoCommand {
    public:
        shared_ptr<NoExpression> condition;
        shared_ptr<NoCommand> then_cmd;
        shared_ptr<NoCommand> else_cmd;

        NoConditional(shared_ptr<NoExpression> cond,
                      shared_ptr<NoCommand> then_cmd,
                      shared_ptr<NoCommand> else_cmd = nullptr);
        
        void print() override;
};

class NoRepetition : public NoCommand {
    public:
        shared_ptr<NoExpression> condition;
        shared_ptr<NoCommand> body;

        NoRepetition(shared_ptr<NoExpression> cond,
                     shared_ptr<NoCommand> body);
    
        void print() override;
};

class NoRead : public NoCommand {
    public:
        vector<string> identifier_list;

        NoRead(vector<string> list);
        
        void print() override;
};

class NoWrite : public NoCommand {
    public:
        vector<shared_ptr<NoExpression>> expression_list;

        NoWrite(vector<shared_ptr<NoExpression>> list);
    
        void print() override;
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
