// =============================================================================
// Tablas/Atributos auxiliares asumidos en GenCodeVisitor:
// =============================================================================
// - std::unordered_map<std::string, int> memoria;    // Mapea variables a su offset en la pila (ya existe en visitor.h)
// - std::unordered_map<std::string, int> matrixCols; // Mapea el nombre de la matriz a su número de columnas (Cols)
// - int offset;                                      // Próximo offset en bytes disponible en la pila local (ya existe en visitor.h)
// - std::ostream &out;                               // Stream de salida (ya existe en visitor.h)
// =============================================================================

// 1. Creación y asignación de memoria en el Heap (usando malloc)
// Representa la declaración de la matriz (ej. matrix[rows][cols])
int GenCodeVisitor::visit(MatrixDecl* stm) {
    if (memoria.find(stm->name) == memoria.end()) {
        // Reservamos un slot en la pila para guardar el puntero retornado por malloc
        memoria[stm->name] = offset;
        matrixCols[stm->name] = stm->cols;
        offset -= 8; // Solo se decrementa 8 bytes en la pila local para almacenar el puntero
        
        int totalElements = stm->rows * stm->cols;
        
        // Genera el código para calcular el tamaño en bytes e invocar a malloc@PLT
        out << "  movq $" << totalElements << ", %rax\n";
        out << "  movq $8, %rcx\n";
        out << "  imulq %rcx, %rax\n";
        out << "  movq %rax, %rdi\n";
        out << "  call malloc@PLT\n";
        
        // Guarda el puntero retornado en %rax en el slot asignado de la pila
        out << "  movq %rax, " << memoria[stm->name] << "(%rbp)\n";
    }
    return 0;
}

// 2. Expresión de acceso a un elemento de la matriz en el Heap (ej: matrix[rowExpr][colExpr])
// Calcula la dirección base del heap + index * 8 y carga el valor a %rax
int GenCodeVisitor::visit(MatrixAccessExp* exp) {
    // 1. Evalúa rowExpr (el resultado queda en %rax)
    exp->rowExpr->accept(this);
    
    // 2. Multiplica por el número de columnas (Cols) de la matriz
    int cols = matrixCols[exp->name];
    out << "  imulq $" << cols << ", %rax\n";
    
    // 3. Guarda el resultado temporal (rowExpr * Cols) en la pila
    out << "  pushq %rax\n";
    
    // 4. Evalúa colExpr (el resultado queda en %rax)
    exp->colExpr->accept(this);
    
    // 5. Recupera el resultado temporal en %rcx y suma colExpr
    out << "  popq %rcx\n";
    out << "  addq %rcx, %rax\n"; // %rax = rowExpr * Cols + colExpr (índice lineal k)
    
    // 6. Carga el puntero base del heap en %rcx
    out << "  movq " << memoria[exp->name] << "(%rbp), %rcx\n";
    
    // 7. Carga el valor en %rax usando la dirección indexada (%rcx, %rax, 8)
    out << "  movq (%rcx, %rax, 8), %rax\n";
    
    return 0;
}

// 3. Sentencia de asignación a un elemento de la matriz en el Heap (ej: matrix[rowExpr][colExpr] = valueExpr)
// Sigue la convención: %rax = base pointer, %rdi = índice (offset lineal), %rcx = valor
int GenCodeVisitor::visit(MatrixAssignStm* stm) {
    // 1. Evalúa rowExpr (el resultado queda en %rax)
    stm->rowExpr->accept(this);
    
    // 2. Multiplica por el número de columnas (Cols) de la matriz
    int cols = matrixCols[stm->name];
    out << "  imulq $" << cols << ", %rax\n";
    
    // 3. Guarda el resultado temporal (rowExpr * Cols) en la pila
    out << "  pushq %rax\n";
    
    // 4. Evalúa colExpr (el resultado queda en %rax)
    stm->colExpr->accept(this);
    
    // 5. Recupera el resultado temporal en %rcx y suma colExpr
    out << "  popq %rcx\n";
    out << "  addq %rcx, %rax\n"; // %rax tiene el índice lineal (k)
    
    // 6. Guarda el índice temporal en la pila
    out << "  pushq %rax\n";
    
    // 7. Evalúa la expresión a asignar valueExpr (el resultado queda en %rax)
    stm->valueExpr->accept(this);
    
    // 8. Pasa el valor a %rcx
    out << "  movq %rax, %rcx\n";
    
    // 9. Recupera el índice en %rdi
    out << "  popq %rdi\n";
    
    // 10. Carga el puntero base en %rax desde la pila
    out << "  movq " << memoria[stm->name] << "(%rbp), %rax\n";
    
    // 11. Escribe el valor (%rcx) en la dirección calculada (%rax, %rdi, 8)
    out << "  movq %rcx, (%rax, %rdi, 8)\n";
    
    return 0;
}
