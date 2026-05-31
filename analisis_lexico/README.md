# Analizador Léxico de Palabras Latinas usando DFA

## Descripción

El lenguaje elegido consiste en dos palabras provenientes de las lenguas latinas: “aud” y “regul”.

Tras un análisis superficial, el alfabeto es:

Σ = {a, e, d, g, l, r, u}

La técnica de modelado utilizada es un Autómata Finito Determinista (DFA), ya que es la más práctica para un análisis léxico que involucra símbolos latinos (Hopcroft, Motwani, & Ullman, 2001).

---

## Modelo del Analizador Léxico

Existen dos autómatas para este idioma:

### 1. Autómata para “aud”

El primer autómata representa la palabra “aud”, mostrando la concatenación secuencial de los símbolos:

## Autómata para “aud”

![DFA for aud](images/dfa_aud.png)

### 2. Autómata para “regul”

El segundo autómata representa la palabra “regul”, mostrando igualmente la concatenación secuencial de los símbolos:

## Autómata para “regul”

![DFA for regul](images/dfa_regul.png)

Estos dos autómatas son suficientes para este contexto; son capaces de determinar el idioma en cuestión, así como el alfabeto relevante.

### Expresiones Regulares Equivalentes

Conversión de DFA a Expresión Regular:

* DFA → RE 1: `(aud)+`
* DFA → RE 2: `(regul)+`

---

## Implementación

Según las expresiones regulares definidas en `regexp_latin.ipynb` y la función `regex_latin_word_identifier`, se puede realizar una búsqueda para identificar el idioma.

Asimismo, en el archivo `dfa_latin.pl` se encuentra la implementación de un programa/autómata en Prolog diseñado para reconocer el alfabeto en cuestión. Mediante reglas como `contiene_patron` y `encontrar`, el programa es capaz de escanear una secuencia de caracteres latinos y detectar los patrones relevantes para la tarea.


A través de una frase o texto que contenga latín como entrada, el programa determinará si dicho texto contiene una o ambas palabras.

### Ejemplos

"Discipuli regulam audiunt" → Aprobada

"Postea regulam novam audimus et meminimus." → Aprobada

"Regula123 audiunt456" → Reprobada

---

## Pruebas

El archivo `regexp_latin.ipynb` contiene 20 casos de prueba utilizando las expresiones regulares definidas para validar el correcto funcionamiento del analizador léxico.

---

## Análisis de Tiempo

### Autómata en Prolog

La solución implementada en Prolog realiza un escaneo secuencial sobre una lista de caracteres que representa el texto latino. El objetivo del programa es detectar si dentro del texto aparece alguno de los patrones relevantes del proyecto, como `aud` o `regul`.

Sea:

- `n` = número total de caracteres en el texto de entrada.
- `m` = longitud del patrón que se desea reconocer.

En este proyecto, los patrones son pequeños y fijos:

- `aud` tiene longitud 3.
- `regul` tiene longitud 5.

Por lo tanto, `m` puede considerarse constante.

El programa revisa la lista desde el primer símbolo hasta el último. En cada posición, intenta verificar si el patrón comienza ahí. Si no encuentra coincidencia, avanza al siguiente símbolo y repite el proceso.

En el peor caso, el patrón no aparece en el texto, por lo que el programa debe revisar todas las posiciones de la entrada. Como los patrones tienen longitud fija, la comparación en cada posición toma tiempo constante.

Por esta razón, la complejidad temporal asintótica de la solución es:

$$
O(n)
$$  

Esto significa que el tiempo de ejecución crece linealmente con respecto al tamaño del texto de entrada.

### Expresión regular
La complejidad de mi modelo (`regex_latin_word_identifier`) es de eficiencia asintótica lineal \(O(n)\) (Levitin, 2012), donde \(n\) es la longitud de la cadena ingresada. 

Según el plan general en (Levitin,2012,p.62) para un análisis de complejidad, resulta:

El número de operaciones realizadas por el algoritmo es:

$$
C(n) = \sum_{i=1}^{n-1} 1 = n - 1
$$

Por lo tanto, la complejidad asintótica es:

$$
\Theta(n)
$$


Esto articula la eficiencia del autómata que modela el reconocimiento de este alfabeto.

Se utilizó la biblioteca `re` o `regex`, instalada a través de **Anaconda Navigator**. El uso de expresiones regulares permite alcanzar una alta velocidad para cumplir con la meta de reconocer el alfabeto en cuestión.

Una solución alternativa sería utilizar bucles anidados donde se considere cada elemento de la cadena introducida. Sin embargo, este enfoque resultaría computacionalmente más costoso, con complejidades aproximadas de \(O(n^2)\) o incluso \(O(n^3)\).

---

## Referencias

Hopcroft, J. E., Motwani, R., & Ullman, J. D. (2001). *Introduction to automata theory, languages, and computation* (2nd ed.). Addison-Wesley.

Levitin, A. (2012). *Introduction to the design and analysis of algorithms* (3rd ed.). Pearson Education.
