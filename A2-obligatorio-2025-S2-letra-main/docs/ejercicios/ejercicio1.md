# Ejercicio 1 - Sistema de Rankings de Jugadores

## Descripción

Estás desarrollando un sistema de rankings para una plataforma de gaming en línea. El sistema debe mantener un registro eficiente de jugadores con sus puntajes y permitir consultas rápidas sobre las posiciones en el ranking. Cada jugador tiene un ID único, un nombre de usuario y un puntaje actual.

## Entrada

- La primera línea contiene un entero `N` (1 ≤ N ≤ 10^6), que representa la cantidad de operaciones a realizar.
- Las siguientes `N` líneas contienen las operaciones:

  - `ADD <ID> <Nombre> <Puntaje>`: Agregar un jugador. Si el ID ya existe, no hacer nada.
  - `FIND <ID>`: Buscar un jugador por su ID.
  - `RANK <Puntaje>`: Contar jugadores con puntaje ≥ al especificado.
  - `TOP1`: Obtener el jugador con mayor puntaje (menor ID en caso de empate).
  - `COUNT`: Obtener la cantidad total de jugadores registrados.

## Salida

- `FIND`: Imprimir `{nombre} {puntaje}` si existe, o `jugador_no_encontrado` si no existe.
- `RANK`: Imprimir la cantidad de jugadores con puntaje ≥ al especificado.
- `TOP1`: Imprimir `{nombre} {puntaje}` del líder, o `sin_jugadores` si no hay jugadores.
- `COUNT`: Imprimir el número total de jugadores registrados.

## Restricciones

- `1 <= N <= 10^6`
- `1 <= ID <= 10^8`
- `0 <= Puntaje <= 10^6`
- Nombres de hasta 50 caracteres (sin espacios).
- `ADD` y `FIND`: $O(\log N)$
- `RANK`: $O(\log N)$
- `TOP1` y `COUNT`: $O(1)_{pc}$
- Se debe utilizar un árbol AVL como estructura de datos principal.

## Ejemplo

### Input

```
11
ADD 101 Alice 750
ADD 202 Bob 850
ADD 303 Charlie 600
FIND 202
TOP1
COUNT
RANK 700
ADD 404 Diana 850
TOP1
ADD 202 Bobby 900
COUNT
```

### Output

```
Bob 850
Bob 850
3
2
Bob 850
4
```
