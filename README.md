# Obligatorio de Estructuras de Datos y Algoritmos 2

Trabajo obligatorio de la materia Estructuras de Datos y Algoritmos 2, Ingeniería en Sistemas, Universidad ORT Uruguay (2025). Hecho en equipo de dos, en C++ sin STL para las estructuras: todos los TADs están implementados desde cero.

## Estructuras implementadas (`tads/`)

- **AVL** — árbol binario de búsqueda balanceado
- **Hash abierto y hash cerrado** — tablas con encadenamiento y con direccionamiento abierto
- **Min-heap y max-heap** — con variantes para aristas (Kruskal) y para potencias
- **MFSet (union-find)** — conjuntos disjuntos con unión y búsqueda
- **Lista** genérica

## Ejercicios

| # | Técnica | Estructura |
|---|---------|------------|
| 1 | Inserción, búsqueda y recorridos | AVL |
| 2 | Conteo con tabla hash | Hash |
| 3 | Conteo con tabla hash | Hash abierto |
| 4 | K-way merge / selección | Min-heap |
| 5 | Árbol de cubrimiento mínimo (Kruskal) | MFSet + min-heap de aristas |
| 6 | Optimización con cola de prioridad | Max-heap |
| 7 | Merge sort con conteo de intercambios | Hash cerrado |
| 8 | Programación dinámica con memoización 3D | — |
| 9 | Programación dinámica | — |
| 10 | Backtracking con poda | — |

Cada ejercicio lee de entrada estándar y escribe en salida estándar. Los casos de prueba están en `tests/ejercicioN/` con entradas de 10 hasta 1.000.000 de elementos, más casos especiales.

## Compilar y probar

```bash
g++ -std=c++11 -O2 -o ejercicio5 ejercicio5.cpp
./ejercicio5 < tests/ejercicio5/1000.in.txt | diff - tests/ejercicio5/1000.out.txt
```

El informe con el análisis de complejidad de cada solución está en `eya informe-2.pdf`.
