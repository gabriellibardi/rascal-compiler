#pragma once

#include <vector>
#include <memory>
#include <string>

using namespace std;

class No {
    public:
        virtual ~No();
        virtual void print();
};

class NoProgram : public No {
    public:
        string test;
        void print() override;

        NoProgram(string test);
        ~NoProgram();
};

extern shared_ptr<NoProgram> root;
