# Ejercicio 5: Árbol de Cubrimiento Mínimo

## Descripción

Dado un grafo no dirigido con pesos en las aristas, encontrar el peso total del **Árbol de Cubrimiento Mínimo (Minimum Spanning Tree)** utilizando **Kruskal** con **DisjointSet optimizado**.

Si el grafo no es conexo, se debe encontrar el bosque de cubrimiento mínimo (suma de los árboles de cubrimiento mínimo de cada componente conexa).

## Entrada

La primera línea contiene dos enteros $V$ y $E$ ($1 \leq V \leq 10^5$, $0 \leq E \leq 5 \times 10^5$), el número de vértices y aristas respectivamente.

Las siguientes $E$ líneas contienen tres enteros $u$, $v$, $w$ ($0 \leq u, v < V$, $1 \leq w \leq 10^6$), representando una arista entre los vértices $u$ y $v$ con peso $w$.

## Salida

Un único entero representando el peso total del árbol (o bosque) de cubrimiento mínimo.

## Restricciones

- $1 \leq V \leq 10^5$
- $0 \leq E \leq 5 \times 10^5$
- $1 \leq w \leq 10^6$
- Los vértices están numerados de $0$ a $V-1$
- Usar algoritmo de Kruskal con DisjointSet optimizado
- No puede haber aristas múltiples entre el mismo par de vértices

## Complejidad Esperada

- **Tiempo**: $O((V + E) \log V)$
- **Espacio**: $O(V + E)$

