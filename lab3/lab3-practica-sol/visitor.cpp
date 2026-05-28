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

int IdExp::accept(Visitor *visitor) {
    return visitor->visit(this);
}

int NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int SqrtExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int PrintStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
}

int AssignStmt::accept(Visitor* visitor) {
    return visitor->visit(this);
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


void PrintVisitor::imprimir(Program* programa){
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


void EVALVisitor::interprete(Program* programa){
    if (programa)
    {
        cout << "Interprete:" << programa->accept(this)<<endl;
    }
    return;

}

///////////////////////////////////

int EVALVisitor::visit(Program *p) {
    for (auto i:p->slist) {
        i->accept(this);
    }
    return 0;
}

int EVALVisitor::visit(AssignStmt *stm) {
    int v = stm->e->accept(this);
    memoria[stm->id] = v;
    return 0;
}

int EVALVisitor::visit(PrintStmt *stm) {
    int v = stm->e->accept(this);
    cout << v << endl;
    return 0;
}

int PrintVisitor::visit(Program *p) {
    for (auto i:p->slist) {
        i->accept(this);
    }
    return 0;
}

int PrintVisitor::visit(AssignStmt *p) {
    cout << p->id << " = ";
    p->e->accept(this);
    cout << endl;
    return 0;
}

int PrintVisitor::visit(PrintStmt *p) {
    cout << "print ( ";
    p->e->accept(this);
    cout << " ) " << endl;
    return 0;
}

int PrintVisitor::visit(IdExp *exp) {
    cout << exp->id;
    return 0;
}

int EVALVisitor::visit(IdExp *exp) {
    return memoria[exp->id];
}