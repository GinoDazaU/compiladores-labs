  ### 1. El Mecanismo del Doble Despacho (Double Dispatch)
  El patrón funciona en dos partes interconectadas:

  • Los nodos del AST (ast.h):
  Cada nodo de expresión ( Exp ) y sentencia ( Stm ) declara un método virtual puro  accept(Visitor* visitor) .
  • La implementación de  accept  (visitor.cpp):
  En lugar de verificar de qué tipo es el nodo usando  dynamic_cast  o estructuras condicionales, cada clase de AST
  implementa  accept  redirigiendo la ejecución de vuelta al visitor pasándose a sí misma ( this ):
    int BinaryExp::accept(Visitor* visitor) {
        return visitor->visit(this); // Redirige al método visit específico para BinaryExp
    }
  Esto permite que el compilador resuelva dinámicamente en tiempo de ejecución qué método sobrecargado  visit(...)
  de la clase  Visitor  debe invocarse.
  ──────
  ### 2. La Interfaz Base:  Visitor  (visitor.h)

  La clase abstracta  Visitor  define las firmas necesarias para visitar cada elemento concreto del AST:

  • Para expresiones ( Exp ):  NumberExp ,  BinaryExp  e  IdExp .
  • Para sentencias ( Stm ):  AssignStm ,  PrintStm ,  IfStm  y  WhileStm .
  ──────
  ### 3. El Generador de Código:  GencodeVisitor  (visitor.cpp)

  Esta es la clase concreta que hereda de  Visitor  y se encarga de generar instrucciones ensamblador x86-64 en la
  consola (salida estándar).

  #### Estado interno de  GencodeVisitor :

  •  posicion : Un mapa ( unordered_map<string,int> ) que asocia el identificador de cada variable con una posición
  en el marco de pila (stack frame).
  •  contador : Un entero para asignar nuevas direcciones en la pila para variables locales (comienza en  1  e
  incrementa con cada nueva variable).

  #### Ciclo de vida y procesamiento:

  1. Punto de entrada ( gencode ):
  Para iniciar el proceso, se llama a visitor.cpp. Esta función:
      • Genera el prólogo del archivo ensamblador ( .data ,  .text , etiqueta  main: ).
      • Prepara el marco de pila ( pushq %rbp ,  movq %rsp, %rbp ) y reserva espacio para variables locales ( subq
      $1024, %rsp ).
      • Itera sobre la lista de sentencias del programa ( program->slist ), llamando a  accept(this)  en cada una de
      ellas para delegar el control a sus respectivos métodos  visit .
      • Genera el epílogo ( leave ,  ret ).
  2. Funcionamiento de los métodos  visit  individuales:
      • visitor.cpp: Coloca la constante numérica directamente en el registro  %rax . E.g.,  movq $42, %rax .
      • visitor.cpp: Carga el valor de la variable en  %rax  desde su dirección en la pila ( posicion * -8 ). E.g.,
      movq -8(%rbp), %rax .
      • visitor.cpp:
          1. Evalúa el subárbol izquierdo ( left->accept(this) ) y guarda el resultado de  %rax  en la pila ( pushq
          %rax ).
          2. Evalúa el subárbol derecho ( right->accept(this) ).
          3. Realiza la operación aritmética: recupera el valor de la izquierda en  %rax  ( popq %rax ) moviendo el
          de la derecha temporalmente a  %rcx  y ejecuta la instrucción ensamblador correspondiente (p. ej.,  addq  o
          subq ).
      • visitor.cpp: Si la variable no tiene una posición asignada, se le reserva una nueva usando  contador .
      Luego evalúa la expresión asignada y guarda el resultado ( %rax ) en el espacio de memoria reservado en la pila.
      • visitor.cpp: Evalúa la expresión, coloca el valor obtenido en el registro  %rsi  (segundo parámetro de
      llamada en la convención System V), prepara el formato de impresión en  %rdi  y realiza la llamada a la función
      estándar  printf .
      • visitor.cpp y visitor.cpp: Generan etiquetas de salto ( else: ,  endif: ,  while: ,  endwhile: ) y
      utilizan instrucciones de comparación ( cmpq $0, %rax ) y saltos condicionales ( je ,  jmp ) para emular el
      flujo de control del programa visitando sus respectivos bloques de código ( cuerpo ).


  ### Nota / Detalle a tener en cuenta:

  En ast.h está declarado  Program::accept(Visitor* visitor) , pero no está implementado en
  visitor.cpp. En su lugar se está usando directamente la función de utilidad  GencodeVisitor::gencode(Program*
  program) .