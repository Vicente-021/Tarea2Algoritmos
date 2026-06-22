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

struct Edge {
    int u, v;
    float peso;
};

auto crearListaDeAristas(const string& archivo){
    ifstream archivo_mtx(archivo);
    string linea;
    getline(archivo_mtx, linea);

    vector<Edge> listaDeAristas;

    int nodos, columnas, aristas;
    archivo_mtx >> nodos >> columnas >> aristas;

    listaDeAristas.resize(aristas);

    int u, v;
    float peso;

    while (archivo_mtx >> u >> v >> peso){
        if(u>=1 && u<=nodos && v>=1 && v<=nodos){
            listaDeAristas.push_back({u,v,peso});
        }
    }
    return listaDeAristas;
}



auto crearMatrizAdyacencia(const string& archivo){
    ifstream archivo_mtx(archivo);
    vector<vector<double>> matriz;

    //Eliminar encabezado
    string linea;
    getline(archivo_mtx, linea);


    //Identificar datos necesarios de la primera fila del archivo, tamaño y numero de aristas
    int nodos, columnas, aristas;
    archivo_mtx >> nodos >> columnas >> aristas;
    matriz.resize(nodos, vector<double>(nodos, 0));

    int u, v;
    double peso;
    
    //Iterar el resto de lineas
    while (archivo_mtx >> u >> v >> peso){
        if(u>=1 && u<=nodos && v>=1 && v<=nodos){
            matriz[u-1][v-1]=peso;
        }        
    }

    archivo_mtx.close();
    return matriz;
}

//bellman ford
float algoritmoBase(int fila_start, int col_start, int fila_fin, int col_fin, auto &matriz){
    vector<int> = distancias(n, __INT_MAX__);
    distancia[]
    
    return 0;
}

int main(){
    auto matrizAdy = crearMatrizAdyacencia("Chebyshev2.mtx");

    cout << matrizAdy[0][5];

    return 0;
}