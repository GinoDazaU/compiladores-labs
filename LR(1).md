# LR(0)

## 0. Flujo (leer al final)

Un parser LR(0) funciona mediante un proceso ascendente (bottom-up) que busca reconstruir el árbol sintáctico desde los tokens hasta la raíz. El flujo comienza aumentando la gramática con una regla inicial ($S' \to S$) para definir un punto de parada claro. A alto nivel, el parser utiliza una pila y un autómata de estados que rastrea el progreso de las reglas mediante un "punto" ($\cdot$).El proceso inicia en un estado base donde se calcula la clausura (si el punto está antes de un No-Terminal, se añaden todas sus reglas con el punto al inicio). Al leer un símbolo, el parser realiza un Shift (desplaza el punto a la derecha y cambia de estado) o, si el punto llega al final de una regla, realiza una Reduce (sustituye los símbolos hijos en la pila por el padre). Aunque inicialmente se puede conceptualizar como un Autómata Finito No Determínico (AFN) debido a las transiciones $\epsilon$ que ocurren durante la clausura (donde "saltamos" a las reglas de un No-Terminal sin consumir entrada), en la práctica este se convierte en un Autómata Finito Determínico (AFD) para que el parser pueda tomar decisiones únicas en cada paso sin necesidad de mirar símbolos hacia adelante (lookahead).

## 1. Definición y Mecánica Base
**LR(0)** es un analizador sintáctico **ascendente** (*bottom-up*). Su objetivo es construir el árbol sintáctico desde las hojas (tokens) hacia la raíz (símbolo inicial).

* **L (Left-to-right):** Lee la entrada de izquierda a derecha.
* **R (Rightmost derivation):** Utiliza una derivación a la derecha inversa.
* **0 (Lookahead):** Toma decisiones basándose únicamente en el estado actual, sin consultar el siguiente símbolo de la entrada.

### Acciones del Parser:
1.  **Shift (Desplazar):** Mueve el siguiente token de la entrada a la pila.
2.  **Reduce (Reducir):** Sustituye una secuencia de símbolos en el tope de la pila (el lado derecho de una regla) por su correspondiente No-Terminal (el lado izquierdo).

## 2. Componentes Esenciales

### A. Gramática Aumentada ($G'$)
Para que el parser identifique inequívocamente el estado de aceptación, se añade una nueva regla inicial. Si el símbolo inicial es $S$, se agrega:
$$S' \to S$$
Esto garantiza que el parser se detenga solo cuando haya procesado toda la entrada y reducido el contenido a la raíz original.

### B. Ítems LR(0)
Un ítem es una regla de producción con un punto ($\cdot$) insertado en algún lugar del lado derecho. El punto representa el **progreso** del análisis:
* $A \to \cdot XYZ$: Indica que esperamos ver una cadena derivable de $XYZ$.
* $A \to X \cdot YZ$: Indica que acabamos de reconocer $X$ y ahora esperamos $YZ$.
* $A \to XYZ \cdot$: Indica que hemos reconocido la regla completa y estamos listos para una **reducción**.

### C. Clausura (Closure)
Es una operación que expande un conjunto de ítems. Si el punto está antes de un No-Terminal $B$ ($A \to \alpha \cdot B \beta$), se deben incluir en el estado todos los ítems que produzcan a $B$ con el punto al inicio ($B \to \cdot \gamma$).

### D. Función GOTO (Ir_a)
Define la transición entre estados. Si tenemos un estado $I$ y leemos un símbolo $X$ (terminal o no-terminal), $GOTO(I, X)$ nos mueve a un nuevo estado donde el punto se ha desplazado una posición a la derecha del símbolo $X$.


## 3. Construcción del Autómata (Colección Canónica)
El parser LR(0) se basa en un **Autómata Finito Determínico (DFA)** donde cada estado es un conjunto de ítems.

1.  Se inicia en el Estado 0 con la clausura de $S' \to \cdot S$.
2.  Para cada símbolo que siga a un punto en un estado, se calcula la función GOTO.
3.  Se repite el proceso hasta que no aparezcan nuevos estados.

## 4. Conflictos en LR(0)
Una gramática no es LR(0) si su tabla presenta ambigüedades en un mismo estado:

* **Conflicto Shift/Reduce:** Un estado contiene un ítem listo para reducir ($A \to \alpha \cdot$) y otro ítem que espera un terminal para desplazar ($B \to \beta \cdot a \gamma$). Al no mirar adelante (0 lookahead), el parser no sabe cuál elegir.
* **Conflicto Reduce/Reduce:** Un estado contiene dos o más ítems listos para reducir ($A \to \alpha \cdot$ y $B \to \beta \cdot$). El parser no sabe qué regla aplicar.


## 5. Requisitos de la Gramática
* **No requiere eliminar recursividad por la izquierda:** A diferencia de LL(1), los parsers LR manejan la recursividad por la izquierda de forma natural y eficiente.
* **Determinismo:** La gramática debe ser capaz de ser analizada sin ambigüedad mediante el autómata de estados.