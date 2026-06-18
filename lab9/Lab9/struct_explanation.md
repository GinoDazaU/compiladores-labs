# Implementación de Estructuras (Structs) en el Compilador

Este documento detalla el diseño a alto nivel para soportar **estructuras (structs)** dinámicas en el heap, mapeando campos con offsets fijos en memoria.

## 1. Idea de Alto Nivel (Registro y Offsets Fijos)
Un struct es un tipo de datos compuesto que agrupa variables (campos) de forma contigua en memoria. Cada campo tiene una posición fija relativa al inicio de la estructura.

### Estructura de Memoria (Heap):
Si tenemos `struct Point { int x; int y; }`, una instancia de `Point` en memoria se compone de:
*   `offset + 0`: Campo `x` (8 bytes)
*   `offset + 8`: Campo `y` (8 bytes)
*   Tamaño total: `16` bytes.

### Mecanismo en Compilación:
*   En tiempo de compilación definimos los offsets y el tamaño total de la estructura.
*   En tiempo de ejecución (`runtime`), instanciamos la estructura reservando su tamaño total con `malloc`.
*   Para acceder o escribir en un campo, leemos la dirección base de la estructura y le sumamos el offset estático del campo.

---

## 2. Flujo de Operaciones y Aritmética x86-64

### A. Instanciación (`StructNewExp` / `new Point`)
Reservamos memoria en el heap llamando a `malloc` con el tamaño total calculado del struct. El puntero retornado en `%rax` representa la instancia y se guarda en la pila local.

```
size = structSizes[typeName]
puntero = malloc(size)
```

### B. Acceso a Campo (`StructAccessExp` / `var.field`)
Para leer un campo `var.field`:
1.  Buscamos el offset correspondiente al campo en la tabla de símbolos del struct: `fieldOffset = structFields[structType][field]`.
2.  Cargamos el puntero base del struct en un registro (ej. `%rcx`).
3.  Cargamos en `%rax` el valor ubicado en `fieldOffset(%rcx)`.

```
base = pila[varName]
fieldOffset = structFields[structType][field]
rax = *(base + fieldOffset)
```

### C. Asignación a Campo (`StructAssignStm` / `var.field = value`)
Para escribir un valor en un campo:
1.  Evaluamos la expresión del valor (`valueExpr`), guardando el resultado en un registro temporal (`%rcx`).
2.  Cargamos el puntero base del struct en `%rdx`.
3.  Escribimos en la dirección exacta usando direccionamiento de offset: `movq %rcx, fieldOffset(%rdx)`.

```
rcx = valueExpr
base = pila[varName]
fieldOffset = structFields[structType][field]
*(base + fieldOffset) = rcx
```

---

## 3. Ejemplo de Pseudocódigo Simplificado (Visitor)

```cpp
// Instanciación (Ej: new Point)
int visit(StructNewExp* exp) {
    int size = structSizes[exp->structType];
    emit("movq $" + size + ", %rdi");
    emit("call malloc@PLT"); // %rax tiene la base del struct
    return 0;
}

// Lectura de Campo (Ej: p.y)
int visit(StructAccessExp* exp) {
    emit("movq offset_pila(%rbp), %rcx"); // %rcx = base pointer de la struct
    int fieldOffset = structFields[structType][exp->field];
    emit("movq " + fieldOffset + "(%rcx), %rax"); // %rax = base->field
    return 0;
}

// Escritura de Campo (Ej: p.y = 10)
int visit(StructAssignStm* stm) {
    stm->valueExpr->accept(this);        // %rax = 10
    emit("movq %rax, %rcx");             // %rcx = 10
    emit("movq offset_pila(%rbp), %rdx"); // %rdx = base pointer
    int fieldOffset = structFields[structType][stm->field];
    emit("movq %rcx, " + fieldOffset + "(%rdx)"); // base->field = 10
    return 0;
}
```
