#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
#include <list>
#include <unordered_map>

class BinaryExp;
class NumberExp;
class SqrtExp;

class Visitor {
public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(IdExp* exp) = 0;
    virtual int visit(SqrtExp* exp) = 0;
    virtual void visit(AsignStmt* stm) = 0;
    virtual void visit(PrintStmt* stm) = 0;
    virtual void visit(Programa* program) = 0;
};

class PrintVisitor : public Visitor {
public:

    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(SqrtExp* exp) override;
    void visit(AsignStmt* stm) override;
    void visit(PrintStmt* stm) override;
    void visit(Programa* program) override;
    int visit(IdExp* exp) override;
    void imprimir(Programa* program);
};

class EVALVisitor : public Visitor {
public:
    unordered_map<string,int> memoria;
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(SqrtExp* exp) override;
    void visit(AsignStmt* stm) override;
    void visit(PrintStmt* stm) override;
    int visit(IdExp* exp) override;
    void visit(Programa* program) override;
    void interprete(Programa* program);
};


#endif // VISITOR_H