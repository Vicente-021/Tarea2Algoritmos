# Contexto: Tarea 2 - Análisis de Algoritmos

## Archivo principal
`tarea2.cpp`

## Objetivo
Encontrar el camino más corto entre todo par de nodos en un grafo dirigido con pesos, leyendo datos desde un archivo `.mtx` (MatrixMarket).

## Representaciones del grafo

### Matriz de adyacencia (`crearMatrizAdyacencia`)
- Lee archivos `.mtx` (encabezados con `%`, línea de dimensiones `nodos columnas aristas`, luego aristas como `u v peso`)
- Los índices en `.mtx` son 1-based, la matriz interna es 0-based (`matriz[u-1][v-1]`)
- La matriz es `vector<vector<double>>`, valor `0` = no hay arista
- Bugs corregidos: tipo `int`→`double`, saltar múltiples líneas `%`, condiciones de índices (`>=1 && <=nodos`), ajuste 1-based a 0-based
- Acceso O(1) al peso de cualquier arista
- Para grafos dispersos (como Chebyshev2: 2053 nodos, 18447 aristas vs ~4.2M celdas posibles), es menos eficiente en memoria y para iterar aristas

### Lista de aristas (`struct Edge`)
- Struct auxiliar con atributos `u`, `v` y `peso`
- Función para leer el archivo `.mtx` y generar `vector<Edge>`
- Más eficiente para Bellman-Ford: O(V·E) vs O(V³) con matriz
- Mejor para grafos dispersos

## Archivos de prueba
- `Chebyshev2.mtx`: grafo real (2053 nodos, 18447 aristas)
- `test.mtx`: grafo de prueba (4 nodos, 5 aristas)

## Algoritmos a implementar

### 1. `algoritmoBase` (V llamadas a Bellman-Ford)

**Bellman-Ford con lista de aristas:**
```
bellmanFord(aristas, nodos, s):
    dist = array de tamaño nodos, todos INF
    dist[s] = 0

    // Relajar V-1 veces
    for i = 1 to nodos-1:
        for cada Edge(u, v, peso) en aristas:
            if dist[u] != INF and dist[u] + peso < dist[v]:
                dist[v] = dist[u] + peso

    // Detectar ciclo negativo (opcional)
    for cada Edge(u, v, peso) en aristas:
        if dist[u] != INF and dist[u] + peso < dist[v]:
            return "Ciclo negativo"

    return dist
```

**Para all-pairs shortest paths:**
```
for s = 0 to nodos-1:
    resultados[s] = bellmanFord(aristas, nodos, s)
```

Cada ejecución se diferencia por el nodo fuente `s`:
- `s = 0` → caminos más cortos desde el nodo 0 hacia todos los demás
- `s = 1` → caminos más cortos desde el nodo 1 hacia todos los demás
- ...y así hasta `s = nodos-1`

El resultado final es una matriz donde `resultados[i][j]` = distancia más corta del nodo `i` al nodo `j`.

**Complejidad:**
- Con lista de aristas: O(V²·E)
- Con matriz de adyacencia: O(V⁴)

### 2. `algoritmoFloydWarshall`
Algoritmo de Floyd-Warshall para all-pairs shortest paths
- Complejidad: O(V³)
- Más eficiente que el enfoque de V llamadas a Bellman-Ford
- Natural con matriz de adyacencia (acceso O(1) a `matriz[i][k]` y `matriz[k][j]`)

## Estado actual
- `crearMatrizAdyacencia` funciona correctamente
- `struct Edge` implementado para lista de aristas
- Falta implementar ambos algoritmos y comparar resultados

## Consideraciones
- La matriz usa `0` para indicar "no hay arista"
- Si existen aristas con peso 0, se necesitaría un valor centinela (ej: `INF` o `NaN`) para distinguir
- La función `imprimirMatriz` muestra solo las primeras 10 filas para facilitar la visualización
- Para grafos dispersos (como Chebyshev2), la lista de aristas es más eficiente para Bellman-Ford
- Para Floyd-Warshall, la matriz de adyacencia es natural y eficiente
