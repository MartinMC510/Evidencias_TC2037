# Concurrent Word Frequency Analyzer in C++

## Descripción

Este proyecto implementa un **analizador concurrente de frecuencia de palabras** en C++. El problema consiste en recibir una lista de palabras, dividirla en secciones y contar cuántas veces aparece cada palabra usando múltiples threads.

Este problema es útil porque el conteo de palabras aparece en áreas como:

- Procesamiento de texto
- Análisis de documentos
- Motores de búsqueda
- Procesamiento de lenguaje natural
- Análisis de logs

El proyecto se resuelve principalmente usando el **paradigma de programación concurrente**, visto en clase. En este paradigma, varias tareas pueden ejecutarse al mismo tiempo o aparentar ejecutarse al mismo tiempo mediante threads. En C++, esto se demuestra mediante la librería `<thread>`, usando objetos `thread`, funciones ejecutadas por cada thread, y el método `join()` para esperar a que los threads terminen.

Además, el programa usa un `mutex` para proteger el acceso a un recurso compartido. Esto es importante porque, si varios threads modifican una misma estructura de datos al mismo tiempo, puede ocurrir una **race condition**.

---

## Modelos

### Lógica General de la Solución

El programa divide el procesamiento en dos fases principales:

1. **Conteo local**
   - Cada thread recibe una parte del vector de palabras.
   - Cada thread crea su propio contador local.
   - Esto evita que varios threads modifiquen directamente el resultado global mientras cuentan.

2. **Combinación de resultados**
   - Cada thread combina su resultado local con una estructura global llamada `resultado`.
   - Esta parte se protege con un `mutex` porque `resultado` es un recurso compartido.

---

### Diagrama del Paradigma Concurrente

```text
Vector de palabras
      |
      v
División en partes
      |
      v
+-------------------+      +-------------------+
| thread 1            |      | thread 2            |
| Cuenta palabras   |      | Cuenta palabras   |
| en su sección     |      | en su sección     |
+-------------------+      +-------------------+
      |                            |
      v                            v
Mapa local 1                 Mapa local 2
      |                            |
      +-------------+--------------+
                    |
                    v
          Resultado global combinado con mutex
                    |
                    v
          Frecuencia final de palabras
```

---

### Diagrama de Arquitectura del Programa

```text
main()
 |
 |-- Crea vector de palabras
 |
 |-- Define número de threads
 |
 |-- Crea threads con std::thread
 |      |
 |      |-- thread 1 -> contarPalabras()
 |      |-- thread 2 -> contarPalabras()
 |
 |-- Espera los threads con join()
 |
 |-- Ordena el resultado con sort()
 |
 |-- Imprime frecuencia final
```

---

### Recurso Compartido

El recurso compartido principal es:

```cpp
vector<pair<string, int>> resultado;
```

Esta estructura funciona como el **contador global de palabras**. Cada elemento contiene:

```text
palabra -> frecuencia
```

Por ejemplo:

```text
ho -> 3
aner -> 3
blepei -> 2
```

Como `resultado` puede ser modificado por más de un thread, se protege con:

```cpp
mtx.lock();
...
mtx.unlock();
```

---

## Implementación

El programa fue implementado en **C++**. Lo cual se encuentra en el archivo `main.cpp`.


---

## Pruebas

Una prueba fue realizada con palabras del griego antiguo. Con esta prueba se verifica que el programa cuente correctamente las palabras y combine los resultados de los threads sin perder información.

---

### Prueba 1: Conteo básico con palabras repetidas

#### Entrada

```text
ho aner blepei ton doulon
ho aner pherei ton doulon
ho aner blepei en to agro
```

Representado en el programa como:

```cpp
vector<string> palabras = {
    "ho", "aner", "blepei", "ton", "doulon",
    "ho", "aner", "pherei", "ton", "doulon",
    "ho", "aner", "blepei", "en", "to", "agro"
};
```

#### Resultado esperado

| Palabra | Frecuencia esperada |
|---------|---------------------|
| ho | 3 |
| aner | 3 |
| blepei | 2 |
| ton | 2 |
| doulon | 2 |
| pherei | 1 |
| en | 1 |
| to | 1 |
| agro | 1 |

#### Resultado esperado en consola

```text
Frecuencia de palabras:
ho: 3
aner: 3
blepei: 2
ton: 2
doulon: 2
pherei: 1
en: 1
to: 1
agro: 1
```

El orden puede variar para palabras con la misma frecuencia, pero los conteos han de coincidir.


---

## Análisis

### Complejidad temporal de la solución concurrente

Sea:

- `n` = número total de palabras
- `p` = número de threads
- `u` = número de palabras únicas

El programa divide el vector de palabras entre los threads. Idealmente, cada thread procesa aproximadamente:

```text
n / p
```

palabras.

Sin embargo, como se usa un `vector<pair<string, int>>` para buscar palabras, la función `buscarPalabra()` tiene complejidad:

```text
O(u)
```

porque puede revisar varias palabras hasta encontrar una coincidencia.

Por lo tanto, el conteo local puede acercarse a:

```text
O((n / p) * u)
```

Después, los resultados locales se combinan en el vector global `resultado`. Esa fase también depende del número de palabras únicas, porque se vuelve a buscar cada palabra antes de agregarla o actualizarla.

Finalmente, el programa ordena los resultados usando `sort()`, cuya complejidad es:

```text
O(u log u)
```

Por lo tanto, una forma general de expresar la complejidad es:

```text
O((n / p) * u + u log u)
```

En el peor caso, si todas las palabras son diferentes, entonces `u` puede acercarse a `n`.

---

### Ventaja del paradigma concurrente

La ventaja principal de esta solución es que el trabajo de conteo se divide entre varios threads. Esto puede mejorar el rendimiento cuando el volumen de datos es grande y la computadora tiene varios núcleos disponibles.

Además, el diseño evita modificar directamente el resultado global durante el conteo. Cada thread primero trabaja con su propio resultado local y solamente entra a la sección crítica cuando necesita combinar sus resultados.

Esto reduce el tiempo que los threads pasan esperando por el `mutex`.

---

### Posible solución usando otro paradigma

El mismo problema también podría resolverse usando el **paradigma imperativo secuencial**.

En una solución secuencial, el programa usaría un solo ciclo para recorrer todas las palabras y actualizar un único contador global.

Ejemplo conceptual:

```text
Para cada palabra:
    buscar palabra en resultado
    si existe:
        aumentar contador
    si no existe:
        agregar palabra con frecuencia 1
```

Esta solución sería más simple porque no necesita threads, `join()`, ni `mutex`.


---

### Comparación final

La solución concurrente puede ser más eficiente cuando `n` es grande y hay varios núcleos disponibles, ya que divide el conteo entre varios threads. Sin embargo, también introduce complejidad adicional por el manejo de threads, el uso de `join()` y la necesidad de proteger recursos compartidos con `mutex`.

La solución secuencial es más sencilla y menos propensa a errores de concurrencia, pero no aprovecha múltiples threads.

En conclusión, el paradigma concurrente es apropiado para este problema porque el conteo de palabras puede dividirse naturalmente en partes independientes. Cada thread puede procesar una sección distinta del texto y luego combinar sus resultados con seguridad usando un mutex.

---

## Bibliografía

- Stroustrup, B. (2013). *The C++ Programming Language* (4th ed.). Addison-Wesley Professional.  
  Usado como referencia para conceptos del lenguaje C++.
- Weiss, M. A. (2014). *Data Structures and Algorithm Analysis in C++* (4th ed.). Pearson/Addison-Wesley.  
  Usado como referencia para el análisis de algoritmos-- complejidad logarítmica.