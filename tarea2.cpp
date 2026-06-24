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
#include <algorithm>

using namespace std;

struct Edge {
    int u, v;
    double peso;
};

const double INF = 1e18;

vector<Edge> crearListaDeAristas(const string& archivo){
    ifstream archivo_mtx(archivo);
    string linea;
    getline(archivo_mtx, linea);

    int nodos, columnas, aristas;

    archivo_mtx >> nodos >> columnas >> aristas;
    vector<Edge> listaDeAristas;

    int u, v;
    double peso;

    while (archivo_mtx >> u >> v >> peso){
        if(u>=1 && u<=nodos && v>=1 && v<=nodos){
            listaDeAristas.push_back({u-1,v-1,peso});
        }
    }
    return listaDeAristas;
}

vector<vector<double>> crearMatrizAdyacencia(const string& archivo){
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

vector<double> Bellmanford(int origen, const vector<Edge> &aristas, int nodos, bool& tieneCicloNegativo){
    tieneCicloNegativo = false;
    vector<double> distancias(nodos, INF);
    distancias[origen] = 0;

    for(int i=0; i<nodos-1;i++){
        for(Edge arista : aristas){
            if(distancias[arista.u]<INF){
                distancias[arista.v] = min(distancias[arista.v], distancias[arista.u] + arista.peso);
            }
        }
    }

    for(const Edge& arista : aristas){
        if(distancias[arista.u]<INF && distancias[arista.u]+arista.peso < distancias[arista.v]){
            tieneCicloNegativo = true;
            break;
        }
    }


    return distancias;
}


//bellman ford n veces

int main(){
    vector<Edge> aristas = crearListaDeAristas("test_negcycle.mtx");
    int n = 4;
    bool cicloNeg = false;
    vector<double> dist = Bellmanford(0, aristas, n, cicloNeg);

    cout << "Ciclo negativo: " << (cicloNeg? "si" : "no") << "\n";
    for(int i=0; i<n;i++){
        cout << "Dist["<< i <<"] = " << dist[i] << "\n";
    }
    return 0;
}