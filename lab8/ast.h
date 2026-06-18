#ifndef AST_H
#define AST_H

#include <string>
#include <unordered_map>
#include <list>
#include <ostream>

using namespace std;

class Visitor; 

// Operadores binarios soportados (y operadores nuevos para la prueba escrita)
enum BinaryOp { 
    PLUS_OP, 
    MINUS_OP, 
    MUL_OP, 
    DIV_OP,
    POW_OP,
    LE_OP,
    // --- NUEVO: Prueba Escrita (Operadores lógicos y de relación) ---
    LT_OP,  // < (Menor que)
    GT_OP,  // > (Mayor que)
    GE_OP,  // >= (Mayor o igual que)
    EQ_OP,  // == (Igualdad)
    NE_OP,  // != (Diferencia)
    AND_OP, // && (Y lógico)
    OR_OP   // || (O lógico)
};

// Clase abstracta Exp
class Exp {
public:
    virtual int  accept(Visitor* visitor) = 0;
    virtual ~Exp() = 0;  // Destructor puro → clase abstracta
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

// Expresión de Identificador
class IdExp : public Exp {
public:
    string value;
    int accept(Visitor* visitor);
    IdExp(string v);
    ~IdExp();
};


class Stm{
public:
    virtual int accept(Visitor* visitor) = 0;
    virtual ~Stm() = 0;
};

// --- NUEVO: Prueba Escrita (Representa un bloque de sentencias - "body") ---
class Body : public Stm {
public:
    list<Stm*> slist;
    Body();
    ~Body();
    void add(Stm* s);
    int accept(Visitor* visitor) override;
};

class AssignStm: public Stm {
public:
    string id;
    Exp* e;
    AssignStm(string, Exp*);
    ~AssignStm();
    int accept(Visitor* visitor);
};

class PrintStm: public Stm {
public:
    Exp* e;
    PrintStm(Exp*);
    ~PrintStm();
    int accept(Visitor* visitor);
};

class IfStm: public Stm {
public:
    Exp* e;
    Stm* cuerpo1; // NUEVO: Tipo Stm* (apuntando a un Body) en lugar de list<Stm*>
    Stm* cuerpo2; // NUEVO: Tipo Stm* (apuntando a un Body) en lugar de list<Stm*>
    IfStm(Exp* cond, Stm* c1, Stm* c2 = nullptr);
    ~IfStm();
    int accept(Visitor* visitor);
};

class WhileStm: public Stm {
public:
    Exp* e;
    Stm* cuerpo; // NUEVO: Tipo Stm* (apuntando a un Body) en lugar de list<Stm*>
    WhileStm(Exp* cond, Stm* c);
    ~WhileStm();
    int accept(Visitor* visitor);
};

// --- NUEVO: Prueba Escrita (Sentencia break para romper bucles) ---
class BreakStm : public Stm {
public:
    BreakStm();
    ~BreakStm();
    int accept(Visitor* visitor) override;
};

// --- NUEVO: Prueba Escrita (Bucle Do-While) ---
class DoWhileStm : public Stm {
public:
    Stm* cuerpo; // Apunta a un Body
    Exp* e;      // Condición
    DoWhileStm(Stm* c, Exp* cond);
    ~DoWhileStm();
    int accept(Visitor* visitor) override;
};

// Estructura de Caso para Switch
struct SwitchCase {
    int value;
    Stm* cuerpo; // Apunta a un Body
};

// --- NUEVO: Prueba Escrita (Estructura de control Switch) ---
class SwitchStm : public Stm {
public:
    Exp* e;                 // Expresión a evaluar
    list<SwitchCase> cases; // Casos
    Stm* defaultCuerpo;     // Caso default (opcional)
    SwitchStm(Exp* cond, list<SwitchCase> cs, Stm* def = nullptr);
    ~SwitchStm();
    int accept(Visitor* visitor) override;
};

class Program{
public:
    list<Stm*> slist;
    Program();
    void add(Stm*);
    ~Program();
    int accept(Visitor* visitor);
};

#endif // AST_H
