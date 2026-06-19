// input: Grafo dirigido con pesos y 2 nodos inicio:s y fin:t
// output: El camino más corto y su peso con BELLMAN-FORD

// Ahora debemos encontrar el menor camino entre todo par de nodos. FLOYD-WARSHALL
// Ese hay q implementar.

// algoritmoBase = n llamadas de BELLMAN FORD --- Pagina 651 del Cormen
// algortimoFloydWarshall =  --- Pagina 693 del Cormen

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

auto crearMatrizAdyacencia(const string& archivo){
    ifstream archivo_mtx(archivo);
    vector<vector<double>> matriz;

    //eliminar encabezado
    string linea;
    getline(archivo_mtx, linea);



    int nodos, columnas, aristas;
    archivo_mtx >> nodos >> columnas >> aristas;
    matriz.resize(nodos, vector<double>(nodos, 0));

    int u, v;
    double peso;
    
    while (archivo_mtx >> u >> v >> peso){
        if(u>=1 && u<=nodos && v>=1 && v<=nodos){
            matriz[u-1][v-1]=peso;
        }        
    }

    archivo_mtx.close();
    return matriz;
}

//bellman ford
float algoritmoBase(){
    return 0;
}

int main(){
    auto matrizAdy = crearMatrizAdyacencia("Chebyshev2.mtx");
    
    cout << matrizAdy[5][1];

    return 0;
}