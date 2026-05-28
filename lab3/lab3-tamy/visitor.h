#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
#include <list>
#include <unordered_map>

class BinaryExp;
class NumberExp;
class SqrtExp;
class StringExp;

class Visitor {
public:
    virtual Value visit(BinaryExp* exp) = 0;
    virtual Value visit(NumberExp* exp) = 0;
    virtual Value visit(IdExp* exp) = 0;
    virtual Value visit(SqrtExp* exp) = 0;
    virtual Value visit(StringExp* exp) = 0;
    virtual void visit(AsignStmt* stm) = 0;
    virtual void visit(PrintStmt* stm) = 0;
    virtual void visit(Programa* program) = 0;
};

class PrintVisitor : public Visitor {
public:
    Value visit(BinaryExp* exp) override;
    Value visit(NumberExp* exp) override;
    Value visit(SqrtExp* exp) override;
    Value visit(StringExp* exp) override;
    Value visit(IdExp* exp) override;
    void visit(AsignStmt* stm) override;
    void visit(PrintStmt* stm) override;
    void visit(Programa* program) override;
    void imprimir(Programa* program);
};

class EVALVisitor : public Visitor {
public:
    unordered_map<string,Value> memoria;
    Value visit(BinaryExp* exp) override;
    Value visit(NumberExp* exp) override;
    Value visit(SqrtExp* exp) override;
    Value visit(StringExp* exp) override;
    Value visit(IdExp* exp) override;
    void visit(AsignStmt* stm) override;
    void visit(PrintStmt* stm) override;
    void visit(Programa* program) override;
    void interprete(Programa* program);
};


#endif // VISITOR_H