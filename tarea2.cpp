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

//PRUEBAS
#include <iomanip>
using namespace std;

struct Edge {
    int u, v;
    double peso;
};

const double INF = 1e18;

pair<int, vector<Edge>> crearListaDeAristas(const string& archivo){
    ifstream archivo_mtx(archivo);
    if (!archivo_mtx.is_open()) { cerr << "Error no se pudo abrir el archivo"; exit(1); }
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
    archivo_mtx.close();
    return {nodos, listaDeAristas};
}

vector<vector<double>> crearMatrizAdyacencia(const string& archivo){
    ifstream archivo_mtx(archivo);
    if (!archivo_mtx.is_open()) { cerr << "Error: no se pudo abrir el archivo"; exit(1); }
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

    //calcular distancias
    for(int i=0; i<nodos-1;i++){
        for(const Edge& arista : aristas){
            if(distancias[arista.u]<INF){
                distancias[arista.v] = min(distancias[arista.v], distancias[arista.u] + arista.peso);
            }
        }
    }

    //ciclos negativos
    for(const Edge& arista : aristas){
        if(distancias[arista.u]<INF && distancias[arista.u]+arista.peso < distancias[arista.v]){
            tieneCicloNegativo = true;
            break;
        }
    }
    return distancias;
}


//bellman ford n veces

vector<vector<double>> algoritmoBase(const vector<Edge> &aristas, int nodos, bool& tieneCicloNegativo){
    tieneCicloNegativo = false;
    vector<vector<double>> resultadoAlgBase(nodos, vector<double>(nodos, INF));
    for(int i=0; i<nodos;i++){
        bool cicloNeg = false;
        resultadoAlgBase[i] = Bellmanford(i, aristas, nodos, cicloNeg);
        tieneCicloNegativo = tieneCicloNegativo || cicloNeg;
    }
    return resultadoAlgBase;
}

double encontrarMinimo(const vector<vector<double>> &matriz){
    double minimo = INF;
    int nodos = matriz.size();

    for(int i=0;i<nodos;i++){
        for(int j=0;j<nodos;j++){
            if (i!=j && matriz[i][j]<minimo && matriz[i][j]<INF){
                minimo = matriz[i][j];
            }
        }
    }
    return minimo;
}

int main(){
    auto [n, aristas] = crearListaDeAristas("test_negcycle.mtx");
    bool tieneCicloNegativo = false;
    vector<vector<double>> matriz = algoritmoBase(aristas, n, tieneCicloNegativo);
    cout << "Matriz de distancias (algoritmoBase):\n";
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(matriz[i][j] >= INF){
                cout << "  INF";
            }
            else{
                cout << setw(5) << matriz[i][j];
            }
        }
        cout << "\n";
    }
    cout << "\nDistancia minima entre todo par: " << encontrarMinimo(matriz) << "\n";
    cout << "Ciclo negativo detectado: " << (tieneCicloNegativo ? "si" : "no") << "\n";
    return 0;
}