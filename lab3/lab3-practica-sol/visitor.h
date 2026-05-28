#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
#include <list>
#include <unordered_map>
class BinaryExp;
class NumberExp;
class SqrtExp;
class Program;
class PrintStmt;
class AssignStmt;


class Visitor {
public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(IdExp* exp) = 0;
    virtual int visit(SqrtExp* exp) = 0;
    virtual int visit(Program* p) = 0;
    virtual int visit(PrintStmt* stm) = 0;
    virtual int visit(AssignStmt* stm) = 0;
};

class PrintVisitor : public Visitor {
public:

    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(SqrtExp* exp) override;
    int visit(Program* p) override;
    int visit(PrintStmt* stm) override;
    int visit(IdExp* exp) override;
    int visit(AssignStmt* stm) override;
    void imprimir(Program* program);
};

class EVALVisitor : public Visitor {
public:

    unordered_map<string,int> memoria;
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(SqrtExp* exp) override;
    int visit(IdExp* exp) override;
    int visit(Program* p) override;
    int visit(PrintStmt* stm) override;
    int visit(AssignStmt* stm) override;
    void interprete(Program* program);
};



#endif // VISITOR_H