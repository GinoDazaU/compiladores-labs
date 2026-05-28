#include <iostream>
#include <fstream>
#include <cmath>
#include "ast.h"
#include "visitor.h"


using namespace std;

///////////////////////////////////////////////////////////////////////////////////
int BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int SqrtExp::accept(Visitor* visitor) {
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

int PrintVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << ' ' << Exp::binopToChar(exp->op) << ' ';
    exp->right->accept(this);
    return 0;
}

int PrintVisitor::visit(NumberExp* exp) {
    cout << exp->value;
    return 0;
}

int PrintVisitor::visit(SqrtExp* exp) {
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

///////////////////////////////////////////////////////////////////////////////////
int EVALVisitor::visit(BinaryExp* exp) {
    int result;
    int v1 = exp->left->accept(this);
    int v2 = exp->right->accept(this);
    switch (exp->op) {
        case PLUS_OP:
            result = v1 + v2;
            break;
        case MINUS_OP:
            result = v1 - v2;
            break;
        case MUL_OP:
            result = v1 * v2;
            break;
        case DIV_OP:
            if (v2 != 0)
                result = v1 / v2;
            else {
                cout << "Error: división por cero" << endl;
                result = 0;
            }
            break;
        case POW_OP:
            result = pow(v1,v2);
            break;
        default:
            cout << "Operador desconocido" << endl;
            result = 0;
    }
    return result;
}

int EVALVisitor::visit(NumberExp* exp) {
    return exp->value;
}

int EVALVisitor::visit(SqrtExp* exp) {
    return floor(sqrt( exp->value->accept(this)));
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
    memoria[stm->variable]=stm->exp->accept(this);
}

int EVALVisitor::visit(IdExp *e) {
    return memoria[e->value];
}


void EVALVisitor::visit(PrintStmt *stm) {
    cout << stm->exp->accept(this);
}

void EVALVisitor::visit(Programa *p) {
    for (auto i:p->slist) {
        i->accept(this);
    }
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

int PrintVisitor::visit(IdExp *e) {
    cout << e->value;
    return 0;
}

