#include <iostream>
#include <fstream>
#include <cmath>
#include "ast.h"
#include "visitor.h"


using namespace std;

///////////////////////////////////////////////////////////////////////////////////
Value BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

Value NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

Value IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

Value SqrtExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

Value StringExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

void PrintStmt::accept(Visitor* visitor) {
    visitor->visit(this);
}

void AsignStmt::accept(Visitor* visitor) {
    visitor->visit(this);
}

void Programa::accept(Visitor* visitor) {
    visitor->visit(this);
}

///////////////////////////////////////////////////////////////////////////////////

Value PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

Value PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

Value PrintVisitor::visit(SqrtExp* exp) {
    cout << "sqrt(";
    exp->value->accept(this);
    cout <<  ")";
    return 0;
}


void PrintVisitor::imprimir(Programa* programa){
    if (programa)
    {
        cout << "Codigo:" << endl;
        programa->accept(this);
        cout << endl;
    }
    return ;
}

void PrintVisitor::visit(AsignStmt *stm) {
    cout << stm->variable << " = ";
    stm->exp->accept(this);
    cout << endl;
}
void PrintVisitor::visit(PrintStmt *stm) {
    cout << "print (";
    stm->exp->accept(this);
    cout << ")"<< endl;
}

void PrintVisitor::visit(Programa * p) {
    for (auto i:p->slist) {
        i->accept(this);
    }
}

Value PrintVisitor::visit(IdExp *e) {
    cout << e->value;
    return 0;
}

Value PrintVisitor::visit(StringExp* exp) {
    cout << "\"" << exp->value << "\"";
    return Value(0);
}

///////////////////////////////////////////////////////////////////////////////////
Value EVALVisitor::visit(BinaryExp* exp) {
    Value result;
    Value v1 = exp->left->accept(this);
    Value v2 = exp->right->accept(this);
    switch (exp->op) {
        case PLUS_OP:
            result = Value(v1.i + v2.i);
            break;
        case MINUS_OP:
            result = Value(v1.i - v2.i);
            break;
        case MUL_OP:
            result = Value(v1.i * v2.i);
            break;
        case DIV_OP:
            if (v2.i != 0)
                result = Value(v1.i / v2.i);
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        case POW_OP:
            result = Value(pow(v1.i,v2.i));
            break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

Value EVALVisitor::visit(NumberExp* exp) {
    return Value(exp->value);
}

Value EVALVisitor::visit(SqrtExp* exp) {
    Value v = exp->value->accept(this);
    if (v.type == Value::T_INT) {
        return Value((int)floor(sqrt(v.i)));
    }
    cout << "Error: sqrt requiere un numero" << endl;
    return Value(0);
}
Value EVALVisitor::visit(IdExp *e) {
    return memoria[e->value];
}

void EVALVisitor::interprete(Programa* programa){
    if (programa)
    {
        cout << "Interprete:";
        programa->accept(this);
        cout <<endl;
    }
    return;
}

void EVALVisitor::visit(AsignStmt *stm) {
    memoria[stm->variable]=stm->exp->accept(this).i;
}



void EVALVisitor::visit(PrintStmt *stm) {
    Value v =  stm->exp->accept(this);
    if(v.type == Value::T_INT) cout << v.i;
    else cout << v.s;
}

void EVALVisitor::visit(Programa *p) {
    for (auto i:p->slist) {
        i->accept(this);
    }
}

Value EVALVisitor::visit(StringExp* exp) {
    return Value(exp->value);
}
