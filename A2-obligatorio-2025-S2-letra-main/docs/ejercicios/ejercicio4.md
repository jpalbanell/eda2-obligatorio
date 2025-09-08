# Ejercicio 4: Fusión de K Listas Ordenadas

## Descripción

Implementar un algoritmo eficiente para fusionar $K$ listas ordenadas de forma creciente en una sola lista ordenada, utilizando un **min heap**.

Cada lista está ordenada de forma creciente y el resultado final debe ser una lista única que contenga todos los elementos de las $K$ listas, también ordenada de forma creciente.

## Entrada

La primera línea contiene un entero $K$ ($1 \leq K \leq 1000$), el número de listas.

Para cada lista $i$ de $1$ a $K$:
- Una línea con un entero $N_i$ ($0 \leq N_i \leq 10000$), el tamaño de la lista $i$
- $N_i$ líneas, cada una con un entero (elementos ordenados de forma creciente)

## Salida

Los elementos fusionados en orden creciente, **un número por línea**.

Si no hay elementos en ninguna lista, no imprimir nada.

## Restricciones

- $1 \leq K \leq 1000$
- $0 \leq N_i \leq 10000$ para cada lista $i$
- Elementos en el rango $[-10^6, 10^6]$
- Cada lista individual está ordenada de forma creciente
- Usar MinHeap para lograr complejidad $O(N \log K)$ donde $N$ es el total de elementos

## Complejidad Esperada

- **Tiempo**: $O(N \log K)$ donde $N$ es el total de elementos y $K$ el número de listas
- **Espacio**: $O(K)$ para el heap

## Ejemplo

### Input
```
3
4
1
3
5
7
2
2
6
3
0
4
8
```

### Output
```
0
1
2
3
4
5
6
7
8
```

### Explicación

Se tienen 3 listas:
- Lista 1: [1, 3, 5, 7]
- Lista 2: [2, 6] 
- Lista 3: [0, 4, 8]

El resultado fusionado es: [0, 1, 2, 3, 4, 5, 6, 7, 8], impreso un número por línea.
