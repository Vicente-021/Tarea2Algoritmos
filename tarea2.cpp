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
#include <cmath>

//PRUEBAS
#include <iomanip>
using namespace std;

struct Edge {
    int u, v;
    double peso;
};

const double INF = 1e18;

//Parsear las matrices

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

vector<vector<double>> listaAMatriz(const vector<Edge>& aristas, int nodos) {
    vector<vector<double>> matriz(nodos, vector<double>(nodos, INF));
    for (const Edge& a : aristas) {
        matriz[a.u][a.v] = a.peso;
    }
    for (int i = 0; i < nodos; i++) matriz[i][i] = 0;
    return matriz;
}

//Algoritmo base y bellman ford n veces

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

//Floyd warshall

vector<vector<double>> algoritmoFloydWarshall(const vector<Edge>& aristas, int nodos, bool& tieneCicloNegativo) {
    tieneCicloNegativo = false;
    auto d = listaAMatriz(aristas, nodos);

    for (int k = 0; k < nodos; k++) {
        for (int i = 0; i < nodos; i++) {
            for (int j = 0; j < nodos; j++) {
                if (d[i][k] < INF && d[k][j] < INF && d[i][k] + d[k][j] < d[i][j]) {
                    d[i][j] = d[i][k] + d[k][j];
                }
            }
        }
    }

    for (int i = 0; i < nodos; i++) {
        if (d[i][i] < 0) {
            tieneCicloNegativo = true;
            break;
        }
    }

    return d;
}

//Funciones Auxiliares

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

void imprimirMatriz(const vector<vector<double>>& matriz, const string& nombre) {
    int n = matriz.size();
    cout << nombre << ":\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matriz[i][j] >= INF) {
                cout << "  INF";
            } else {
                cout << setw(7) << matriz[i][j];
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

int main(){
    auto [n, aristas] = crearListaDeAristas("test_neg.mtx");

    bool cicloNegBase = false;
    bool cicloNegFW = false;

    vector<vector<double>> matrizBase = algoritmoBase(aristas, n, cicloNegBase);
    vector<vector<double>> matrizFW = algoritmoFloydWarshall(aristas, n, cicloNegFW);

    cout << "Ciclo negativo (algoritmoBase):          " << (cicloNegBase ? "si" : "no") << "\n";
    cout << "Ciclo negativo (algoritmoFloydWarshall): " << (cicloNegFW ? "si" : "no") << "\n\n";

    imprimirMatriz(matrizBase, "Matriz algoritmoBase");
    imprimirMatriz(matrizFW, "Matriz algoritmoFloydWarshall");

    bool iguales = true;
    for (int i = 0; i < n && iguales; i++) {
        for (int j = 0; j < n && iguales; j++) {
            if (abs(matrizBase[i][j] - matrizFW[i][j]) > 1e-9) {
                iguales = false;
                cout << "Dif en [" << i << "][" << j << "]: base=" << matrizBase[i][j] << ", FW=" << matrizFW[i][j] << "\n";
            }
        }
    }
    cout << "Las matrices " << (iguales ? "coinciden" : "DIFIEREN") << "\n";

    return 0;
}