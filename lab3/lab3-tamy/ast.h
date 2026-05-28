#ifndef AST_H
#define AST_H

#include <string>
#include <unordered_map>
#include <list>
#include <ostream>

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

struct Value{
    enum Type { T_INT, T_STR, T_VOID};
    Type type;
    int i = 0;
    string s = "";
  
    Value(int i) : type(Type::T_INT), i(i) {}
    Value(string s) : type(Type::T_STR), s(s) {}
    Value() : type(Type::T_VOID) {}
};

// Clase abstracta Exp
class Exp {
public:
    virtual Value accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;  // Destructor puro → clase abstracta
    static string binopToChar(BinaryOp op);  // Conversión operador → string
};

// Expresión binaria
class BinaryExp : public Exp {
public:
    Exp* left;
    Exp* right;
    BinaryOp op;
    Value accept(Visitor* visitor);
    BinaryExp(Exp* l, Exp* r, BinaryOp op);
    ~BinaryExp();

};

// Expresión numérica
class NumberExp : public Exp {
public:
    int value;
    Value accept(Visitor* visitor);
    NumberExp(int v);
    ~NumberExp();
};


class IdExp : public Exp {
public:
    string value;
    Value accept(Visitor* visitor);
    IdExp(string v);
    ~IdExp();
};

// Raiz cuadrada
class SqrtExp : public Exp {
public:
    Exp* value;
    Value accept(Visitor* visitor);
    SqrtExp(Exp* v);
    ~SqrtExp();
};

class StringExp: public Exp {
public:
    string value;
    Value accept(Visitor* visitor);
    StringExp(string s);
    ~StringExp();
};

class Stmt{
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

#endif // AST_H