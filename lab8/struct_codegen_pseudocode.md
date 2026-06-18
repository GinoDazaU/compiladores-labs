# Diseño a Alto Nivel: Generación de Código para Estructuras (Structs) en el Heap

Este documento explica de forma conceptual y de alto nivel cómo dar soporte a estructuras (`structs`) y acceso a sus campos en un compilador que genera ensamblador x86-64, utilizando asignación dinámica en el **heap** (con `malloc`) para almacenar los datos del struct, de manera análoga al manejo de colecciones y matrices dinámicas.

---

## 1. La Idea Central: Abstracción vs. Representación Física

Desde la perspectiva del programador, un `struct` agrupa múltiples variables bajo un solo tipo de dato (por ejemplo, `struct Point { x; y; }`). Sin embargo, a nivel de ensamblador, un struct en el heap es simplemente un bloque contiguo de memoria de un tamaño determinado por la cantidad de campos que posee.

Para implementar esto en el compilador sin punteros a punteros, se divide la tarea en dos partes:
*   **En la pila (Stack):** Se almacena únicamente un puntero (de 8 bytes) que apunta a la dirección de memoria en el heap donde yacen los campos del struct.
*   **En el heap:** Se aloja el bloque de memoria física real que contendrá todos los campos de forma secuencial.

---

## 2. Gestión de Tipos y Alineación en Memoria

Para calcular la dirección física de un campo (como `p.y`), se necesita conocer el tamaño y el orden de los campos del struct. 

### Registro del Tipo (Metadata)
Cuando el compilador encuentra una declaración de estructura (`struct NombreStruct { ... }`), no se genera código ensamblador. En su lugar, el compilador registra de forma interna los metadatos de la estructura:
*   La asociación del nombre del struct a la secuencia de sus campos (ej. `Point` -> `["x", "y"]`).

### Instanciación y Asignación de Memoria
Cuando se declara una variable de tipo struct (ej. `struct Point p;`):
1.  Se reserva una única posición (8 bytes) en la pila del frame actual para guardar el puntero del struct (`posicion["p"] = contador++`).
2.  Se calcula el tamaño en bytes necesario en el heap (`número de campos * 8 bytes`).
3.  Se genera una llamada a `malloc` en tiempo de ejecución.
4.  El puntero de memoria devuelto en `%rax` por `malloc` se almacena en la ranura reservada en la pila para la variable `p`.

---

## 3. Lógica del Generador de Código (Visitor)

La implementación se basa en tres operaciones a nivel de AST y de generación de código:

### A. Definición de la Estructura (Metadata)
Se registra en un mapa asociativo los campos definidos para el struct.
```cpp
void visit(StructDecl* stm) {
    // Registra que el struct tiene ciertos campos en orden
    structTypes[stm->name] = stm->fields;
}
```

### B. Instanciación en el Heap
Genera el código de inicialización dinámica:
1. Reserva ranura local en la pila.
2. Calcula tamaño: `numCampos * 8`.
3. Invoca a `malloc` y guarda el puntero de `%rax` en la pila.
```cpp
void visit(StructVarDecl* stm) {
    posicion[stm->varName] = contador++;
    int totalBytes = structTypes[stm->typeName].size() * 8;
    
    // Generación de x86-64
    cout << "movq $" << totalBytes << ", %rdi" << endl;
    cout << "call malloc@PLT" << endl;
    cout << "movq %rax, " << -8 * posicion[stm->varName] << "(%rbp)" << endl;
}
```

### C. Acceso a Campo (Lectura: `p.x`)
Para obtener el valor de un campo, se realiza un direccionamiento indexado basado en el puntero cargado del struct:
1.  Se busca la variable `p` en la pila para obtener su puntero base en el heap.
2.  Se busca el índice correspondiente al campo (ej. `x` es índice `0`, `y` es índice `1`).
3.  Se calcula el desplazamiento en bytes (`indice * 8`).
4.  Se accede a la memoria y se deposita el valor en `%rax`.
```cpp
int visit(MemberExp* exp) {
    // 1. Obtener tipo y el índice del campo
    string type = varTypes[exp->obj];
    int fieldIndex = obtenerIndiceCampo(type, exp->field);
    int offset = fieldIndex * 8;
    
    // 2. Cargar el puntero base en %rax desde la pila
    cout << "movq " << -8 * posicion[exp->obj] << "(%rbp), %rax" << endl;
    
    // 3. Cargar el valor del heap a %rax usando desplazamiento inmediato
    cout << "movq " << offset << "(%rax), %rax" << endl;
    return 0;
}
```

### D. Asignación a un Campo (Escritura: `p.y = valor`)
Para escribir un valor en un campo del struct:
1.  Se evalúa la expresión del valor a asignar (el resultado se guarda en `%rax`).
2.  Se mueve temporalmente el valor a un registro secundario (como `%rcx`).
3.  Se obtiene el índice del campo para calcular el desplazamiento (`indice * 8`).
4.  Se carga el puntero del struct desde la pila a `%rax`.
5.  Se escribe el valor de `%rcx` en la dirección calculada `offset(%rax)`.
```cpp
void visit(MemberAssignStm* stm) {
    // 1. Evaluar expresión (resultado en %rax)
    stm->e->accept(this);
    
    // 2. Mover el valor a %rcx
    cout << "movq %rax, %rcx" << endl;
    
    // 3. Calcular desplazamiento del campo
    string type = varTypes[stm->obj];
    int offset = obtenerIndiceCampo(type, stm->field) * 8;
    
    // 4. Cargar puntero del struct a %rax
    cout << "movq " << -8 * posicion[stm->obj] << "(%rbp), %rax" << endl;
    
    // 5. Guardar el valor en la dirección del heap
    cout << "movq %rcx, " << offset << "(%rax)" << endl;
}
```

---

## 4. Beneficios del Diseño

*   **Sin sobrecarga en el Stack**: Independientemente del tamaño del struct, este solo consume un slot (8 bytes) en la pila de ejecución, evitando stack overflows por estructuras grandes.
*   **Desplazamiento Estático en el Heap**: Dado que el orden de los campos es conocido en tiempo de compilación, el desplazamiento (`offset`) es constante, requiriendo menos instrucciones en comparación con indexación dinámica en matrices.
