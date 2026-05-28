#ifndef AST_H
#define AST_H

#include <list>
#include <ostream>
#include <string>
#include <unordered_map>

using namespace std;

class Visitor;

// Operadores binarios soportados
enum BinaryOp {
    PLUS_OP,
    MINUS_OP,
    MUL_OP,
    DIV_OP,
    POW_OP
};

// Clase abstracta Exp
class Exp {
   public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;                      // Destructor puro → clase abstracta
    static string binopToChar(BinaryOp op);  // Conversión operador → string
};

// Expresión binaria
class BinaryExp : public Exp {
   public:
    Exp* left;
    Exp* right;
    BinaryOp op;
    int accept(Visitor* visitor);
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();
};

// Expresión numérica
class NumberExp : public Exp {
   public:
    int value;
    int accept(Visitor* visitor);
    NumberExp(int v);
    ~NumberExp();
};

class IdExp : public Exp {
   public:
    string value;
    int accept(Visitor* visitor);
    IdExp(string v);
    ~IdExp();
};

// Raiz cuadrada
class SqrtExp : public Exp {
   public:
    Exp* value;
    int accept(Visitor* visitor);
    SqrtExp(Exp* v);
    ~SqrtExp();
};

class Stmt {
   public:
    virtual void accept(Visitor* visitor) = 0;
    virtual ~Stmt() = 0;
};

class AsignStmt : public Stmt {
   public:
    string variable;
    Exp* exp;
    void accept(Visitor* visitor) override;
    AsignStmt(string, Exp*);
    ~AsignStmt();
};

class PrintStmt : public Stmt {
   public:
    Exp* exp;
    void accept(Visitor* visitor) override;
    PrintStmt(Exp* e);
    ~PrintStmt();
};

class Programa {
   public:
    list<Stmt*> slist;
    void accept(Visitor* visitor);
    ~Programa();
    Programa();
};

#endif  // AST_H
