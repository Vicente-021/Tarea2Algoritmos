// input: Grafo dirigido con pesos y 2 nodos inicio:s y fin:t
// output: El camino más corto y su peso con BELLMAN-FORD

// Ahora debemos encontrar el menor camino entre todo par de nodos. FLOYD-WARSHALL
// Ese hay q implementar.

// algoritmoBase = n llamadas de BELLMAN FORD --- Pagina 651 del Cormen
// algortimoFloydWarshall =  --- Pagina 693 del Cormen

#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

auto crearMatrizAdyacencia(const string& archivo){
    ifstream archivo_mtx(archivo);
    vector<vector<int>> matriz;

    //eliminar el encabezado.
    string linea;
    getline(archivo_mtx, linea);
    
    //el sgundo numero de nodos, creo que representan las columnas de la matriz de adyacencia
    int nodos, columnas, aristas;
    archivo_mtx >> nodos >> columnas >> aristas;
    matriz.resize(nodos, vector<int>(nodos, 0));

    int u, v;
    double peso;
    
    while (archivo_mtx >> u >> v >> peso){
        if(u>0 && u<nodos && v>0 && v<nodos){
            matriz[u][v]=peso;
        }        
    }

    archivo_mtx.close();
    return matriz;
}

//bellman ford
float algoritmoBase(){
    
}



int main(){
    auto matrizAdy = crearMatrizAdyacencia("Chebyshev2.mtx");

    return 0;
}