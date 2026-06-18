#include <iostream>
#include "ast.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;
unordered_map<std::string, int> memoria;
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

int AssignStm::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int PrintStm::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int WhileStm::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int IfStm::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

// --- NUEVO: Implementaciones de accept para la prueba escrita ---
int Body::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int BreakStm::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int DoWhileStm::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}

int SwitchStm::accept(Visitor* visitor) {
    visitor->visit(this);
    return 0;
}
////////////////////////////////////////////////////////////////////
int GencodeVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    cout << "pushq %rax" << endl;
    exp->right->accept(this);
    cout << "movq %rax, %rcx" << endl; // rcx = operando derecho
    cout << "popq %rax" << endl;       // rax = operando izquierdo
    
    switch(exp->op) {
        case PLUS_OP: {
            cout << "addq %rcx, %rax" << endl;
            break;
        }
        case MINUS_OP: {
            cout << "subq %rcx, %rax" << endl;
            break;
        }
        case MUL_OP: {
            // --- NUEVO: Prueba Escrita (Multiplicación x86) ---
            cout << "imulq %rcx, %rax" << endl;
            break;
        }
        case DIV_OP: {
            // --- NUEVO: Prueba Escrita (División x86) ---
            cout << "cqto" << endl;            // Extiende signo de %rax a %rdx:%rax
            cout << "idivq %rcx" << endl;      // Divide %rdx:%rax entre %rcx; cociente en %rax
            break;
        }
        case LE_OP: {
            // --- NUEVO: Prueba Escrita (Menor o Igual que x86) ---
            cout << "cmpq %rcx, %rax" << endl;
            cout << "setle %al" << endl;
            cout << "movzbq %al, %rax" << endl;
            break;
        }
        case LT_OP: {
            // --- NUEVO: Prueba Escrita (Menor que x86) ---
            cout << "cmpq %rcx, %rax" << endl;
            cout << "setl %al" << endl;
            cout << "movzbq %al, %rax" << endl;
            break;
        }
        case GT_OP: {
            // --- NUEVO: Prueba Escrita (Mayor que x86) ---
            cout << "cmpq %rcx, %rax" << endl;
            cout << "setg %al" << endl;
            cout << "movzbq %al, %rax" << endl;
            break;
        }
        case GE_OP: {
            // --- NUEVO: Prueba Escrita (Mayor o Igual que x86) ---
            cout << "cmpq %rcx, %rax" << endl;
            cout << "setge %al" << endl;
            cout << "movzbq %al, %rax" << endl;
            break;
        }
        case EQ_OP: {
            // --- NUEVO: Prueba Escrita (Igualdad x86) ---
            cout << "cmpq %rcx, %rax" << endl;
            cout << "sete %al" << endl;
            cout << "movzbq %al, %rax" << endl;
            break;
        }
        case NE_OP: {
            // --- NUEVO: Prueba Escrita (Diferencia x86) ---
            cout << "cmpq %rcx, %rax" << endl;
            cout << "setne %al" << endl;
            cout << "movzbq %al, %rax" << endl;
            break;
        }
        case AND_OP: {
            // --- NUEVO: Prueba Escrita (Y lógico x86) ---
            cout << "andq %rcx, %rax" << endl;
            break;
        }
        case OR_OP: {
            // --- NUEVO: Prueba Escrita (O lógico x86) ---
            cout << "orq %rcx, %rax" << endl;
            break;
        }
        default:
            cout << "Operador desconocido" << endl;
    }
    return 0;
}

int GencodeVisitor::visit(NumberExp* exp) {
    cout << "movq $" << exp->value << ", %rax" << endl; 
    return 0;
}

int GencodeVisitor::visit(IdExp* exp) {
    cout << "movq " << posicion[exp->value] * -8 << "(%rbp) , %rax" << endl;
    return 0;
}

void GencodeVisitor::visit(AssignStm* stm) {
    if(posicion.find(stm->id) == posicion.end()){
        posicion[stm->id] = contador;
        contador++;
    }
    stm->e->accept(this);
    cout << "movq %rax ," << -8 * posicion[stm->id] << "(%rbp)" << endl;
}

void GencodeVisitor::visit(PrintStm* stm) {
    stm->e->accept(this);
    cout << "movq %rax , %rsi" << endl;
    cout << "leaq print_fmt(%rip), %rdi" << endl;
    cout << "call printf@PLT" << endl;
}

void GencodeVisitor::gencode(Program* program){
    cout << ".data" << endl;
    cout << "print_fmt:.string \"%ld\\n\" " << endl;
    cout << ".text"<< endl;
    cout << ".globl main"<<endl;
    cout << "main:"<<endl;
    cout << "pushq %rbp" << endl;
    cout << "movq %rsp, %rbp" << endl;
    cout << "subq $1024, %rsp" << endl; 
    for(auto i:program->slist){
        if (i) i->accept(this);
    }
    cout << "movq $0, %rax" << endl;
    cout << "leave" << endl;
    cout << "ret" << endl;
    cout << ".section .note.GNU-stack,\"\",@progbits" << endl;
}

void GencodeVisitor::visit(IfStm* stm ) { 
    // --- NUEVO: Prueba Escrita (Solucionar anidamiento usando label_counter único sin prefijo .L) ---
    int id = label_counter++;
    string else_label = "label_else_" + to_string(id);
    string endif_label = "label_endif_" + to_string(id);

    stm->e->accept(this);
    cout << "cmpq $0 , %rax" << endl;
    cout << "je " << else_label << endl;
    
    // --- NUEVO: Prueba Escrita (Uso de accept en cuerpo1 en vez de iterar sobre una lista) ---
    if (stm->cuerpo1) {
        stm->cuerpo1->accept(this);
    }
    cout << "jmp " << endif_label << endl;
    
    cout << else_label << ":" << endl;
    // --- NUEVO: Prueba Escrita (Uso de accept en cuerpo2 en vez de iterar sobre una lista) ---
    if (stm->cuerpo2) {
        stm->cuerpo2->accept(this);
    }
    cout << endif_label << ":" << endl;
}

void GencodeVisitor::visit(WhileStm* stm) {
    // --- NUEVO: Prueba Escrita (Solucionar anidamiento y pila de etiquetas para break) ---
    int id = label_counter++;
    string start_label = "label_while_" + to_string(id);
    string end_label = "label_endwhile_" + to_string(id);

    loop_ends.push_back(end_label); // Registra etiqueta de fin en la pila

    cout << start_label << ":" << endl;
    stm->e->accept(this);
    cout << "cmpq $0, %rax" << endl;  
    cout << "je " << end_label << endl;
    
    // --- NUEVO: Prueba Escrita (Uso de accept en cuerpo) ---
    if (stm->cuerpo) {
        stm->cuerpo->accept(this);
    }
    
    cout << "jmp " << start_label << endl;
    cout << end_label << ":" << endl;

    loop_ends.pop_back(); // Elimina de la pila al salir
}

// --- NUEVO: Prueba Escrita (Implementaciones de los nuevos métodos visit) ---

void GencodeVisitor::visit(Body* stm) {
    // Procesa cada sentencia del cuerpo secuencialmente delegando a su accept
    for (auto i : stm->slist) {
        if (i) i->accept(this);
    }
}

void GencodeVisitor::visit(BreakStm* stm) {
    // Salta a la etiqueta de fin de bucle que esté más arriba en la pila
    if (!loop_ends.empty()) {
        cout << "jmp " << loop_ends.back() << endl;
    } else {
        cout << "# Warning: break fuera de bucle!" << endl;
    }
}

void GencodeVisitor::visit(DoWhileStm* stm) {
    int id = label_counter++;
    string start_label = "label_dowhile_start_" + to_string(id);
    string cond_label = "label_dowhile_cond_" + to_string(id);
    string end_label = "label_dowhile_end_" + to_string(id);

    loop_ends.push_back(end_label); // Registrar en pila para break

    cout << start_label << ":" << endl;
    if (stm->cuerpo) {
        stm->cuerpo->accept(this);
    }

    cout << cond_label << ":" << endl;
    stm->e->accept(this);
    cout << "cmpq $0, %rax" << endl;
    cout << "jne " << start_label << endl;

    cout << end_label << ":" << endl;
    loop_ends.pop_back();
}

void GencodeVisitor::visit(SwitchStm* stm) {
    int id = label_counter++;
    string end_label = "label_endswitch_" + to_string(id);

    // 1. Evalúa expresión y deja el resultado en %rax
    stm->e->accept(this);

    // 2. Comparaciones sucesivas (If-Else chain)
    int case_idx = 0;
    for (auto& c : stm->cases) {
        cout << "cmpq $" << c.value << ", %rax" << endl;
        cout << "je label_case_" << id << "_" << case_idx << endl;
        case_idx++;
    }

    // Si no coincide ninguno, va al default (si existe) o al final
    if (stm->defaultCuerpo) {
        cout << "jmp label_default_" << id << endl;
    } else {
        cout << "jmp " << end_label << endl;
    }

    // 3. Bloques de sentencias de los casos
    case_idx = 0;
    for (auto& c : stm->cases) {
        cout << "label_case_" << id << "_" << case_idx << ":" << endl;
        if (c.cuerpo) {
            c.cuerpo->accept(this);
        }
        cout << "jmp " << end_label << endl; // Break implícito de switch/case
        case_idx++;
    }

    // 4. Default
    if (stm->defaultCuerpo) {
        cout << "label_default_" << id << ":" << endl;
        stm->defaultCuerpo->accept(this);
        cout << "jmp " << end_label << endl;
    }

    cout << end_label << ":" << endl;
}