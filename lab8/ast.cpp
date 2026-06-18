#include "ast.h"
#include <iostream>

using namespace std;

// ------------------ Exp ------------------
Exp::~Exp() {}

string Exp::binopToChar(BinaryOp op) {
    switch (op) {
        case PLUS_OP:  return "+";
        case MINUS_OP: return "-";
        case MUL_OP:   return "*";
        case DIV_OP:   return "/";
        case POW_OP:   return "**";
        case LE_OP:    return "<=";
        // --- NUEVO: Prueba Escrita (Operadores lógicos y de relación) ---
        case LT_OP:    return "<";
        case GT_OP:    return ">";
        case GE_OP:    return ">=";
        case EQ_OP:    return "==";
        case NE_OP:    return "!=";
        case AND_OP:   return "&&";
        case OR_OP:    return "||";
        default:       return "?";
    }
}

// ------------------ BinaryExp ------------------
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp o)
    : left(l), right(r), op(o) {}

    
BinaryExp::~BinaryExp() {
    delete left;
    delete right;
}



// ------------------ NumberExp ------------------
NumberExp::NumberExp(int v) : value(v) {}

NumberExp::~NumberExp() {}


// ------------------idExp ------------------
IdExp::IdExp(string v) : value(v) {}

IdExp::~IdExp() {}

//////////////////////////

Stm::~Stm(){}

PrintStm::~PrintStm(){}

AssignStm::~AssignStm(){}

Program::~Program(){}

PrintStm::PrintStm(Exp* expresion){
    e=expresion;
}

AssignStm::AssignStm(string variable,Exp* expresion){
    id = variable;
    e = expresion;
}

// --- NUEVO: Prueba Escrita (Implementaciones de constructores/destructores nuevos) ---

Body::Body() {}
Body::~Body() {
    for (auto s : slist) {
        delete s;
    }
}
void Body::add(Stm* s) {
    slist.push_back(s);
}

IfStm::IfStm(Exp* cond, Stm* c1, Stm* c2) : e(cond), cuerpo1(c1), cuerpo2(c2) {}
IfStm::~IfStm() {
    delete e;
    delete cuerpo1;
    if (cuerpo2) delete cuerpo2;
}

WhileStm::WhileStm(Exp* cond, Stm* c) : e(cond), cuerpo(c) {}
WhileStm::~WhileStm() {
    delete e;
    delete cuerpo;
}

BreakStm::BreakStm() {}
BreakStm::~BreakStm() {}

DoWhileStm::DoWhileStm(Stm* c, Exp* cond) : cuerpo(c), e(cond) {}
DoWhileStm::~DoWhileStm() {
    delete cuerpo;
    delete e;
}

SwitchStm::SwitchStm(Exp* cond, list<SwitchCase> cs, Stm* def) : e(cond), cases(cs), defaultCuerpo(def) {}
SwitchStm::~SwitchStm() {
    delete e;
    for (auto& c : cases) {
        delete c.cuerpo;
    }
    if (defaultCuerpo) delete defaultCuerpo;
}

Program::Program(){}

void Program::add(Stm* a){
    slist.push_back(a);
}

