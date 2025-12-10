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

class Visitor;

string op_string(Op op);

class No {
    public:
        virtual void accept(Visitor* visitor);

        virtual ~No();
        virtual void print();
};

class NoDeclaration : public No {
    public:
        vector<string> identifier_list;
        VarType var_type;

        void print() override;
        void accept(Visitor* visitor) override;

        NoDeclaration(vector<string> id_list, VarType var_type);
        ~NoDeclaration();
};

class NoCommand : public No {
    public:
        virtual ~NoCommand() = default;
};

class NoSubroutine : public No {
    public:
        string identifier;
        RoutType rout_type;
        VarType return_type;
        vector<shared_ptr<NoDeclaration>> formal_parameters;
        vector<shared_ptr<NoDeclaration>> declaration_section;
        shared_ptr<NoCommand> body;

        void print() override;
        void accept(Visitor* visitor) override;

        NoSubroutine(string identifier, RoutType rout_type, VarType return_type, vector<shared_ptr<NoDeclaration>> formal_param, vector<shared_ptr<NoDeclaration>> decl_section, shared_ptr<NoCommand> body);
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
        void accept(Visitor* visitor) override;

        NoUnaryExpr(shared_ptr<NoExpression> operand, Op op);
        ~NoUnaryExpr() = default;
};

class NoBinExpr : public NoExpression {
    public:
        shared_ptr<NoExpression> left, right;
        Op op;

        void print() override;
        void accept(Visitor* visitor) override;

        NoBinExpr(shared_ptr<NoExpression> left, shared_ptr<NoExpression> right, Op op);
        ~NoBinExpr() = default;
};

class NoVarExpr : public NoExpression {
    public:
        string identifier;

        void print() override;
        void accept(Visitor* visitor) override;

        NoVarExpr(string identifier);
        ~NoVarExpr() = default;
};

class NoLiteralExpr : public NoExpression {
    public:
        int num;
        bool logic;
        VarType type;

        void print() override;
        void accept(Visitor* visitor) override;

        NoLiteralExpr(int num);
        NoLiteralExpr(bool logic);
        ~NoLiteralExpr() = default;
};

class NoCallExpr : public NoExpression {
    public:
        string identifier;
        vector<shared_ptr<NoExpression>> expression_list;

        void print() override;
        void accept(Visitor* visitor) override;

        NoCallExpr(string identifier, vector<shared_ptr<NoExpression>> expression_list);
        ~NoCallExpr() = default;
};

class NoCompositeCommand : public NoCommand {
    public:
        vector<shared_ptr<NoCommand>> command_list;

        NoCompositeCommand(vector<shared_ptr<NoCommand>> cmds);
        
        void print() override;
        void accept(Visitor* visitor) override;
};

class NoAssignment : public NoCommand {
    public:
        string identifier;
        shared_ptr<NoExpression> expr;

        NoAssignment(string id, shared_ptr<NoExpression> expr);
        
        void print() override;
        void accept(Visitor* visitor) override;
};

class NoProcedureCall : public NoCommand {
    public:
        string identifier;
        vector<shared_ptr<NoExpression>> expression_list;

        NoProcedureCall(string id);
        NoProcedureCall(string id, vector<shared_ptr<NoExpression>> exprs);
    
        void print() override;
        void accept(Visitor* visitor) override;
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
        void accept(Visitor* visitor) override;
};

class NoRepetition : public NoCommand {
    public:
        shared_ptr<NoExpression> condition;
        shared_ptr<NoCommand> body;

        NoRepetition(shared_ptr<NoExpression> cond,
                     shared_ptr<NoCommand> body);
    
        void print() override;
        void accept(Visitor* visitor) override;
};

class NoRead : public NoCommand {
    public:
        vector<string> identifier_list;

        NoRead(vector<string> list);
        
        void print() override;
        void accept(Visitor* visitor) override;
};

class NoWrite : public NoCommand {
    public:
        vector<shared_ptr<NoExpression>> expression_list;

        NoWrite(vector<shared_ptr<NoExpression>> list);
    
        void print() override;
        void accept(Visitor* visitor) override;
};

class NoProgram : public No {
    public:
        string identifier;
        vector<shared_ptr<NoDeclaration>> declaration_section;
        vector<shared_ptr<NoSubroutine>> subroutine_section;
        shared_ptr<NoCommand> body;

        void print() override;
        void accept(Visitor* visitor) override;

        NoProgram(vector<shared_ptr<NoDeclaration>> decl_section, vector<shared_ptr<NoSubroutine>> rout_section, shared_ptr<NoCommand> body);
        ~NoProgram();
};

class SymbolTableManager;

class Visitor {
    protected:
        shared_ptr<SymbolTableManager> symbols;
    public:
        virtual void visit(NoDeclaration* no) = 0;
        virtual void visit(NoSubroutine* no) = 0;
        virtual void visit(NoUnaryExpr* no) = 0;
        virtual void visit(NoBinExpr* no) = 0;
        virtual void visit(NoVarExpr* no) = 0;
        virtual void visit(NoLiteralExpr* no) = 0;
        virtual void visit(NoCallExpr* no) = 0;
        virtual void visit(NoCompositeCommand* no) = 0;
        virtual void visit(NoAssignment* no) = 0;
        virtual void visit(NoProcedureCall* no) = 0;
        virtual void visit(NoConditional* no) = 0;
        virtual void visit(NoRepetition* no) = 0;
        virtual void visit(NoRead* no) = 0;
        virtual void visit(NoWrite* no) = 0;
        virtual void visit(NoProgram* no) = 0;

        virtual ~Visitor() = default;
};

extern shared_ptr<NoProgram> root;
