#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"

using namespace std;

// =============================
// Métodos de la clase Parser
// =============================

Parser::Parser(Scanner* sc) : scanner(sc) {
    previous = nullptr;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        throw runtime_error("Error léxico");
    }
}

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;

        if (check(Token::ERR)) {
            throw runtime_error("Error lexico");
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}


// =============================
// Reglas gramaticales
// =============================

Programa* Parser::parseProgram() {
    Programa* ast = parseP();
    if (!isAtEnd()) {
        throw runtime_error("Error sintáctico");
    }
    cout << "Parseo exitoso" << endl;
    return ast;
}


Programa* Parser::parseP() {
    Programa* p =  new Programa();
    p->cuerpo = parseBody();
    return p;
}

Body* Parser::parseBody() {
    Body* cuerpo =  new Body();
    while(check(Token::VAR)){
        cuerpo->vdlist.push_back(parsevardec());
        match(Token::SEMICOL);
    }
    cuerpo->slist.push_back(parsestmt());
    while (match(Token::SEMICOL)) {
        cuerpo->slist.push_back(parsestmt());
    }
    return cuerpo;
}


Vardec* Parser::parsevardec(){
    match(Token::VAR);
    Vardec* vd = new Vardec();
    match(Token::ID);
    vd->tipo = previous->text;
    match(Token::ID);
    vd->variables.push_back(previous->text);
    while(match(Token::COMA)){
        match(Token::ID);
        vd->variables.push_back(previous->text);
    }
    return vd; 
}

Stmt *Parser::parsestmt() {
    Exp* e;
    if (match(Token::PRINT)) {
        match(Token::LPAREN);
        e = parseCEXP();
        match(Token::RPAREN);
        return new PrintStmt(e);
    }
    else if (match(Token::IF)){
        e = parseCEXP();
        IfStmt* ifstm = new IfStmt(e); 
        match(Token::THEN);
        ifstm -> cuerpo_if = parseBody();
        if(match(Token::ELSE)){
            ifstm -> hayelse = true;
            ifstm -> cuerpo_else = parseBody();
        }
        match(Token::ENDIF);
        return ifstm;
    }
    
    else if (match(Token::WHILE)){
        e = parseCEXP();
        WhileStmt* Trampolin = new WhileStmt(e); 
        match(Token::DO);
        Trampolin->cuerpo.push_back(parsestmt());
        while (match(Token::SEMICOL)) {
            Trampolin->cuerpo.push_back(parsestmt());
        }
        match(Token::ENDWHILE);
        return Trampolin;
    }
    else if (match(Token::ID)) {
        string texto = previous->text;
        match(Token::ASSIGN);
        e = parseCEXP();
        return new AsignStmt(texto,e);
    }
}

Exp* Parser::parseCEXP() {
    Exp* l = parseE();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else{
            op = MINUS_OP;
        }
        Exp* r = parseE();
        l = new BinaryExp(l, r, op);
    }
    return l;
}



Exp* Parser::parseE() {
    Exp* l = parseT();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else{
            op = DIV_OP;
        }
        Exp* r = parseT();
        l = new BinaryExp(l, r, op);
    }
    return l;
}


Exp* Parser::parseT() {
    Exp* l = parseF();
    if (match(Token::POW)) {
        BinaryOp op = POW_OP;
        Exp* r = parseF();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp* Parser::parseF() {
    Exp* e; 
    if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::ID)) {
        return new IdExp(previous->text);
    }
    else if (match(Token::LPAREN))
    {
        e = parseCEXP();
        match(Token::RPAREN);
        return e;
    }
    else if (match(Token::SQRT))
    {   
        match(Token::LPAREN);
        e = parseCEXP();
        match(Token::RPAREN);
        return new SqrtExp(e);
    }
    else {
        throw runtime_error("Error sintáctico");
    }
}
