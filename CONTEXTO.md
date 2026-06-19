# Contexto: Tarea 2 - Análisis de Algoritmos

## Archivo principal
`tarea2.cpp`

## Objetivo
Encontrar el camino más corto entre todo par de nodos en un grafo dirigido con pesos, leyendo datos desde un archivo `.mtx` (MatrixMarket).

## Función `crearMatrizAdyacencia`
- Lee archivos `.mtx` (encabezados con `%`, línea de dimensiones `nodos columnas aristas`, luego aristas como `u v peso`)
- Los índices en `.mtx` son 1-based, la matriz interna es 0-based (`matriz[u-1][v-1]`)
- La matriz es `vector<vector<double>>`, valor `0` = no hay arista
- Bugs corregidos: tipo `int`→`double`, saltar múltiples líneas `%`, condiciones de índices (`>=1 && <=nodos`), ajuste 1-based a 0-based

## Archivos de prueba
- `Chebyshev2.mtx`: grafo real (2053 nodos, 18447 aristas)
- `test.mtx`: grafo de prueba (4 nodos, 5 aristas)

## Algoritmos a implementar

### 1. `algoritmoBase`
V llamadas a Bellman-Ford (una por cada nodo fuente)
- Complejidad con lista de aristas: O(V²·E)
- Complejidad con matriz de adyacencia: O(V⁴)

### 2. `algoritmoFloydWarshall`
Algoritmo de Floyd-Warshall para all-pairs shortest paths
- Complejidad: O(V³)
- Más eficiente que el enfoque de V llamadas a Bellman-Ford

## Estado actual
- `crearMatrizAdyacencia` funciona correctamente
- Falta implementar ambos algoritmos y comparar resultados

## Consideraciones
- La matriz usa `0` para indicar "no hay arista"
- Si existen aristas con peso 0, se necesitaría un valor centinela (ej: `INF` o `NaN`) para distinguir
- La función `imprimirMatriz` muestra solo las primeras 10 filas para facilitar la visualización
