# Gramática de Griego Antiguo (Subconjunto Simplificado)

## Descripción

Este proyecto modela un **subconjunto simplificado del griego antiguo**. La gramática se centra en estructuras básicas de oraciones que incluyen sujetos, verbos y objetos, así como frases preposicionales.

El griego antiguo es una lengua altamente flexible con un orden de palabras variable, lo que introduce **ambigüedad y complejidad** en su análisis sintáctico.

---

## Vocabulario del Subconjunto

| Griego | Transliteración | Significado |
|--------|----------------|------------|
| ὁ | ho | the (sujeto) |
| τόν | ton | the (objeto) |
| τό | to | the (neutro) |
| ἀνήρ | aner | man |
| δοῦλον | doulon | slave |
| ἀγρός | agros | field |
| οἶκον | oikon | house |
| βλέπει | blepei | see |
| φέρει | pherei | carries |
| λέγει | legei | says |
| ἐν | en | in |
| πρός | pros | toward |
| καί | kai | and |

---

## Modelos

### Gramática Base

S -> S CONJ S  
S -> NP VP  

NP -> NP CONJ NP  
NP -> NP PP  
NP -> DET N  
NP -> N  

VP -> VP CONJ VP  
VP -> VP PP  
VP -> V  
VP -> V NP  

PP -> P NP  

DET -> ho | ton | to  
N -> aner | doulos | agros | oikos  
V -> blepei | pherei | legei | elaunei  
P -> en | pros  
CONJ -> kai  

### Explicación 

La gramática base fue diseñada como un fragmento simplificado de la estructura oracional del griego antiguo. Incluye frases nominales, frases verbales, frases preposicionales y coordinación. La gramática es intencionalmente problemática en dos aspectos. Primero, contiene recursión por la izquierda en reglas como S → S CONJ S, NP → NP PP y VP → VP PP. Segundo, es ambigua porque una frase preposicional puede adjuntarse ya sea a una frase nominal o a una frase verbal. Por ejemplo, la cadena ‘ho aner pherei ton doulon en to agro’ puede interpretarse como “el hombre lleva al esclavo en el campo” o como “el hombre lleva al esclavo que está en el campo”. Por lo tanto, esta gramática es un modelo base adecuado para la posterior eliminación de ambigüedad y de recursión por la izquierda. 

---

## Eliminación de Ambigüedad

Aquí se elimina la regla:

NP -> NP PP  

Así que la nueva gramática es:

S -> S CONJ S  
S -> NP VP  

NP -> NP CONJ NP  
NP -> DET N  
NP -> N  

VP -> VP CONJ VP  
VP -> VP PP  
VP -> V  
VP -> V NP  

PP -> P NP  

### Árbol

S  
├── NP  
│   ├── DET → ho  
│   └── N → aner  
└── VP  
    ├── VP  
    │   ├── V → pherei  
    │   └── NP  
    │       ├── DET → ton  
    │       └── N → doulon  
    └── PP  
        ├── P → en  
        └── NP  
            ├── DET → to  
            └── N → agro  

---

## Eliminación de Recursividad Izquierda

La gramática contenía recursión por la izquierda en reglas como S → S CONJ S, NP → NP CONJ NP y VP → VP PP. La recursión por la izquierda causa problemas en los analizadores descendentes (top-down), ya que puede provocar recursión infinita.  Para eliminarla, cada regla recursiva por la izquierda se transformó utilizando no terminales auxiliares (S’, NP’, VP’), convirtiendo la gramática en una forma recursiva por la derecha adecuada para el análisis sintáctico:

S -> NP VP S'  
S' -> CONJ S S' | ε  

NP -> DET N NP' | N NP'  
NP' -> CONJ NP NP' | ε  

VP -> V VP' | V NP VP'  
VP' -> CONJ VP VP' | PP VP' | ε  

PP -> P NP  

### Árbol Final

S  
├── NP  
│   ├── DET → ho  
│   ├── N → aner  
│   └── NP' → ε  
├── VP  
│   ├── V → pherei  
│   ├── NP  
│   │   ├── DET → ton  
│   │   ├── N → doulon  
│   │   └── NP' → ε  
│   └── VP'  
│       ├── PP  
│       │   ├── P → en  
│       │   └── NP  
│       │       ├── DET → to  
│       │       ├── N → agro  
│       │       └── NP' → ε  
│       └── VP' → ε  
└── S' → ε  

---

## Implementación

Se utilizó **NLTK en Python** para analizar cadenas de texto con la gramática. Revise el archivo `grammar_test.ipynb`.

---

## Pruebas

### Cadenas Aceptadas

ho aner blepei  
ho aner pherei ton doulon  
ho aner pherei ton doulon en to agro  
aner legei  

### Cadenas Rechazadas

blepei ho aner  
ho pherei aner  
ho aner en to agro  
kai ho aner blepei  
ho aner pherei ton  

---

## Análisis LL(1)

### Cadena aceptada: ho aner blepei

| Paso | Pila | Entrada | Acción |
|------|------|--------|--------|
| 1 | S $ | ho aner blepei $ | Expandir S -> NP VP |
| 2 | NP VP $ | ho aner blepei $ | Expandir NP -> DET N |
| 3 | DET N VP $ | ho aner blepei $ | Match ho |
| 4 | N VP $ | aner blepei $ | Match aner |
| 5 | VP $ | blepei $ | Expandir VP -> V |
| 6 | V $ | blepei $ | Match blepei |
| 7 | $ | $ | Aceptar |

La cadena `ho aner blepei` es aceptada porque sigue la producción `S -> NP VP`. 
El sintagma nominal se deriva como `NP -> DET N` y el sintagma verbal como `VP -> V`. 
El analizador consume completamente la entrada y la pila queda vacía, por lo que la cadena es aceptada.


### Cadena rechazada: blepei ho aner

| Paso | Pila | Entrada | Acción |
|------|------|--------|--------|
| 1 | S $ | blepei ho aner $ | Expandir S |
| 2 | NP VP $ | blepei ho aner $ | Error: NP no inicia con verbo |
| 3 | — | — | Rechazar |

La cadena `blepei ho aner` es rechazada porque la gramática requiere que la oración comience con un sintagma nominal (NP). 
Sin embargo, el primer token es `blepei`, que es un verbo. 
Por lo tanto, no existe una producción válida para expandir NP, y el analizador rechaza la cadena.


---

## Analysis

### Nivel en la jerarquía de Chomsky (antes de las transformaciones)

La gramática original pertenece al nivel **Tipo 2: Gramáticas Libres de Contexto (CFG)** dentro de la jerarquía de Chomsky. Esto se debe a que todas sus producciones siguen la forma general:

A → α

donde A es un símbolo no terminal y α es una cadena de símbolos terminales y/o no terminales.

Es importante destacar que, aunque la gramática inicial presenta **ambigüedad** y **recursividad izquierda**, estas características no modifican su clasificación dentro de la jerarquía de Chomsky. La ambigüedad afecta la cantidad de posibles árboles de derivación para una misma cadena, pero no la estructura formal de las reglas. De igual manera, la recursividad izquierda es una propiedad de la forma de las producciones, pero no altera el tipo de gramática.

Por lo tanto, antes de aplicar cualquier transformación, la gramática se clasifica como una gramática libre de contexto (Tipo 2).

---

### Nivel en la jerarquía de Chomsky (después de las transformaciones)

Después de eliminar la ambigüedad y la recursividad izquierda, la gramática sigue perteneciendo al nivel **Tipo 2: Gramáticas Libres de Contexto (CFG)**.

Las transformaciones realizadas consisten en reescribir las producciones para controlar la ambigüedad y hacer la gramática compatible con técnicas de parsing predictivo, como LL(1). Sin embargo, todas las reglas continúan teniendo la forma A → α, lo que mantiene la gramática dentro de la misma categoría en la jerarquía de Chomsky.

A pesar de que la clasificación no cambia, la nueva gramática presenta mejoras significativas:

- Reducción o eliminación de ambigüedad estructural  
- Eliminación de recursividad izquierda  
- Mayor compatibilidad con parsers descendentes (top-down)  
- Implementación más eficiente en sistemas computacionales  

---

### Implicaciones de la complejidad temporal

#### Antes de las transformaciones

La gramática original presenta dos problemas principales: ambigüedad y recursividad izquierda.

La **ambigüedad** permite múltiples árboles de derivación para una misma cadena, lo que incrementa significativamente el costo computacional del análisis sintáctico. En el peor de los casos, el número de derivaciones puede crecer exponencialmente.

Además, la **recursividad izquierda** genera problemas en parsers descendentes, ya que puede provocar ciclos infinitos o impedir la terminación del algoritmo.

En general, el análisis de gramáticas libres de contexto ambiguas requiere algoritmos más complejos, como CYK o Earley, los cuales presentan una complejidad en el orden de **O(n³)** en el peor caso (Hopcroft et al., 2001).

---

#### Después de las transformaciones

Tras eliminar la ambigüedad (o reducirla significativamente) y la recursividad izquierda, la gramática se vuelve adecuada para parsers predictivos como LL(1).

Los parsers LL(1) operan en tiempo lineal:

O(n)

donde n es la longitud de la cadena de entrada.

Esta mejora se debe a que:

- No se generan múltiples árboles de derivación  
- No se requiere retroceso (backtracking)  
- Las decisiones se toman utilizando un solo símbolo de anticipación (lookahead)  

Por lo tanto, la transformación de la gramática reduce considerablemente la complejidad del análisis sintáctico y mejora su eficiencia en la práctica.

---

### Relación con el lenguaje


El subconjunto de lenguaje utilizado en este proyecto se basa en estructuras simples del griego antiguo, inspiradas en materiales introductorios como *Athenaze*, donde se presentan construcciones básicas de sintagmas nominales y verbales de manera progresiva (Balme & Lawall, 2003).

Esta simplificación permite modelar el lenguaje mediante una gramática formal sin incorporar la complejidad completa del griego antiguo, como flexión, acentos o variaciones morfológicas avanzadas.


---

## Conclusión

La gramática pasa de ser ambigua y difícil de analizar a una estructura eficiente y apta para parsing predictivo, manteniendo su clasificación como CFG.

---

## Bibliografía


- Hopcroft, J., Motwani, R., & Ullman, J. (2001). 
  *Introduction to Automata Theory, Languages, and Computation*.

- Balme, M., & Lawall, G. (2003). 
  *Athenaze: An Introduction to Ancient Greek*.