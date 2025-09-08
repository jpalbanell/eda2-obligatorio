# Ejercicio 2 - Sistema de Cache de Recursos Web

## Descripción

Estás desarrollando un sistema de cache para un navegador web que almacena información sobre recursos visitados recientemente. El sistema debe ser capaz de almacenar y recuperar rápidamente información sobre recursos web usando dominio y path como claves separadas. Cada entrada del cache contiene un dominio, un path, un título y un tiempo de acceso.

## Entrada

- La primera línea contiene un entero `N` (1 ≤ N ≤ 10^6), que representa la cantidad de operaciones a realizar.
- Las siguientes `N` líneas contienen las operaciones:

  - `PUT <dominio> <path> <titulo> <tiempo>`: Agregar o actualizar un recurso en el cache. Si ya existe, actualiza título y tiempo.
  - `GET <dominio> <path>`: Obtener información de un recurso específico.
  - `REMOVE <dominio> <path>`: Eliminar un recurso específico del cache.
  - `CONTAINS <dominio> <path>`: Verificar si un recurso está en el cache.
  - `COUNT_DOMAIN <dominio>`: Contar la cantidad de recursos de un dominio específico.
  - `LIST_DOMAIN <dominio>`: Listar todos los paths de un dominio ordenados por tiempo (más reciente primero).
  - `CLEAR_DOMAIN <dominio>`: Eliminar todos los recursos de un dominio específico.
  - `SIZE`: Obtener la cantidad total de recursos en el cache.
  - `CLEAR`: Vaciar completamente el cache.

## Salida

- `PUT`, `REMOVE`, `CLEAR_DOMAIN`, `CLEAR`: No producen salida.
- `GET`: Imprimir `{titulo} {tiempo}` si existe, o `recurso_no_encontrado` si no existe.
- `CONTAINS`: Imprimir `true` si existe, `false` si no existe.
- `COUNT_DOMAIN`: Imprimir la cantidad de recursos del dominio especificado.
- `LIST_DOMAIN`: Imprimir los paths del dominio separados por espacios, ordenados por tiempo descendente (más reciente primero).
- `SIZE`: Imprimir el número total de recursos en el cache.

## Restricciones

- `1 <= N <= 10^6`
- Dominios de hasta 50 caracteres (sin espacios).
- Paths de hasta 50 caracteres (sin espacios).
- Títulos de hasta 50 caracteres (sin espacios).
- `1 <= Tiempo <= 10^9`
- Se debe utilizar una tabla de hash cerrada con doble hashing.

**Complejidades requeridas:**
- `GET`, `REMOVE`, `CONTAINS`: $O(1)$ promedio
- `PUT`: $O(k)$ donde k es la cantidad de recursos del dominio 
- `COUNT_DOMAIN`, `LIST_DOMAIN`, `CLEAR_DOMAIN`: $O(k)$ donde k es la cantidad de recursos del dominio
- `SIZE`, `CLEAR`: $O(1)$

## Ejemplo

### Input

```
15
PUT google.com /search SearchPage 1000
PUT google.com /images ImagePage 1500
PUT google.com /maps MapsPage 1200
PUT facebook.com /feed FeedPage 2000
SIZE
COUNT_DOMAIN google.com
LIST_DOMAIN google.com
GET google.com /search
CONTAINS yahoo.com /mail
PUT google.com /search UpdatedSearch 1800
LIST_DOMAIN google.com
CLEAR_DOMAIN google.com
SIZE
COUNT_DOMAIN google.com
CLEAR
```

### Output

```
4
3
/images /maps /search
SearchPage 1000
false
/search /images /maps
1
0
```
