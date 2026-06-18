#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
#include <list>

class BinaryExp;
class NumberExp;
class IdExp;
class AssignStm;
class PrintStm;
class Program;
// --- NUEVO: Forward declarations para la prueba escrita ---
class Body;
class BreakStm;
class DoWhileStm;
class SwitchStm;

class Visitor {
public:
    virtual int visit(BinaryExp* exp) = 0;
    virtual int visit(NumberExp* exp) = 0;
    virtual int visit(IdExp* exp) = 0;
    virtual void visit(AssignStm* stm) = 0;
    virtual void visit(PrintStm* stm) = 0;
    virtual void visit(IfStm* stm) = 0;
    virtual void visit(WhileStm* stm) = 0;
    // --- NUEVO: Métodos de visita para la prueba escrita ---
    virtual void visit(Body* stm) = 0;
    virtual void visit(BreakStm* stm) = 0;
    virtual void visit(DoWhileStm* stm) = 0;
    virtual void visit(SwitchStm* stm) = 0;
};


class GencodeVisitor : public Visitor {
public:
    int contador = 1;
    // --- NUEVO: Variables para solucionar el anidamiento y sentencias break ---
    int label_counter = 0;         // Contador único de etiquetas
    list<string> loop_ends;        // Pila de etiquetas de fin de bucle (para break)

    unordered_map<string,int> posicion ;
    void gencode(Program* program);
    int visit(BinaryExp* exp) override;
    int visit(NumberExp* exp) override;
    int visit(IdExp* exp) override;
    void visit(AssignStm* stm) override;
    void visit(PrintStm* stm) override;
    void visit(IfStm* stm) override;
    void visit(WhileStm* stm) override;
    // --- NUEVO: Sobrecargas de visita para la prueba escrita ---
    void visit(Body* stm) override;
    void visit(BreakStm* stm) override;
    void visit(DoWhileStm* stm) override;
    void visit(SwitchStm* stm) override;
};


#endif // VISITOR_H