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

void WhileStmt::accept(Visitor* visitor) {
    visitor->visit(this);
}

void IfStmt::accept(Visitor* visitor) {
    visitor->visit(this);
}

void Vardec::accept(Visitor* visitor) {
    visitor->visit(this);
}

void Body::accept(Visitor* visitor) {
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
        cout << "Interprete:" << endl; 
        programa->accept(this);
        cout <<endl;
    }
    return;
}




void EVALVisitor::visit(PrintStmt *stm) {
    cout << stm->exp->accept(this);
    cout << endl;
}

void EVALVisitor::visit(Programa *p) {
    memoria.clear();
    memoria.add_level();
    p->cuerpo->accept(this);
    memoria.remove_level();
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
    p->cuerpo->accept(this);
}

int PrintVisitor::visit(IdExp *e) {
    cout << e->value;
    return 0;
}

void PrintVisitor::visit(IfStmt *stm) {
    cout << "if ";
    stm->condicion->accept(this);
    cout << " then" << endl;
    stm->cuerpo_if->accept(this);
    if(stm->hayelse){
        cout << "else" << endl;
        stm->cuerpo_else->accept(this);
    }
    cout << "endif" << endl;
}

void EVALVisitor::visit(IfStmt *stm) {
    if(stm->condicion->accept(this) != 0){
        stm->cuerpo_if->accept(this);
    }
    else if (stm->hayelse){
        stm->cuerpo_else->accept(this);
    }

}

void PrintVisitor::visit(WhileStmt *stm) {
    cout << "while ";
    stm->condicion->accept(this);
    cout << " do" << endl;
    for(auto i:stm->cuerpo){
        cout << "  ";
        i->accept(this);
    }
    cout << "endwhile" << endl;
}
    
void EVALVisitor::visit(WhileStmt *stm) {
    while(stm->condicion->accept(this)!=0){
        for(auto i:stm->cuerpo){
        i->accept(this);
    }
    }
}

void PrintVisitor::visit(Vardec *vd) {
    cout << "var " << vd->tipo << " ";
    for(auto i:vd->variables){
        cout << i << ",";
    }
    cout << endl;
}

void EVALVisitor::visit(Vardec *vd) {
    for(auto& i:vd->variables){
        memoria.add_var(i);
    }
}

int EVALVisitor::visit(IdExp *e) {
    return memoria.lookup(e->value);
}

void EVALVisitor::visit(AsignStmt *stm) {
    memoria.update(stm->variable,stm->exp->accept(this));
}


void EVALVisitor::visit(Body * cuerpo) {
    memoria.add_level();
    for (auto i:cuerpo->vdlist) {
        i->accept(this);
    }
    for (auto i:cuerpo->slist) {
        i->accept(this);
    }
    memoria.remove_level();
}
void PrintVisitor::visit(Body * cuerpo) {
    for (auto i:cuerpo->vdlist) {
        i->accept(this);
    }
    for (auto i:cuerpo->slist) {
        i->accept(this);
    }
}