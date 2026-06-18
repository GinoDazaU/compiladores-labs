// Tablas/Atributos auxiliares asumidos en GencodeVisitor:
// unordered_map<string, int> posicion;             // Mapea variables a su ranura en la pila (ya existe en visitor.h)
// unordered_map<string, list<string>> structTypes; // Mapea el nombre del Struct a su lista de campos en orden
// unordered_map<string, string> varTypes;          // Mapea la variable instanciada a su tipo Struct (ej: "p" -> "Point")
// int contador;                                    // Siguiente slot disponible en la pila (ya existe en visitor.h)

// 1. Declaración / Definición de un Struct (Registra metadatos de los campos)
void GencodeVisitor::visit(StructDecl* stm) {
    structTypes[stm->name] = stm->fields;
}

// 2. Instanciación de una variable de tipo Struct (Reserva slots en pila)
void GencodeVisitor::visit(StructVarDecl* stm) {
    if (posicion.find(stm->varName) == posicion.end()) {
        posicion[stm->varName] = contador;
        varTypes[stm->varName] = stm->typeName;
        
        int numFields = structTypes[stm->typeName].size();
        contador += numFields;
    }
}

// 3. Expresión de acceso a un campo (ej: p.x)
int GencodeVisitor::visit(MemberExp* exp) {
    string typeName = varTypes[exp->obj];
    list<string>& fields = structTypes[typeName];
    
    int fieldIndex = 0;
    for (const string& f : fields) {
        if (f == exp->field) {
            break;
        }
        fieldIndex++;
    }
    
    int slot = posicion[exp->obj] + fieldIndex;
    int offset = -8 * slot;
    
    cout << "movq " << offset << "(%rbp), %rax" << endl;
    return 0;
}

// 4. Sentencia de asignación a un campo (ej: p.x = expresión)
void GencodeVisitor::visit(MemberAssignStm* stm) {
    stm->e->accept(this);
    
    string typeName = varTypes[stm->obj];
    list<string>& fields = structTypes[typeName];
    
    int fieldIndex = 0;
    for (const string& f : fields) {
        if (f == stm->field) {
            break;
        }
        fieldIndex++;
    }
    
    int slot = posicion[stm->obj] + fieldIndex;
    int offset = -8 * slot;
    
    cout << "movq %rax, " << offset << "(%rbp)" << endl;
}
