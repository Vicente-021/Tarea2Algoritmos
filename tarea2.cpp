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
#include <cassert>
#include <tuple>
#include <cstdlib>

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

tuple<int, int, double> encontrarParMasCercano(const vector<vector<double>>& matriz){
    int u = 0, v = 1;
    double minimo = INF;
    int n = matriz.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && matriz[i][j] < INF && matriz[i][j] < minimo) {
                u = i;
                v = j;
                minimo = matriz[i][j];
            }
        }
    }
    return {u, v, minimo};
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

bool verificar(const string& archivo) {
    auto [n, aristas] = crearListaDeAristas(archivo);

    bool cicloNegBase = false;
    bool cicloNegFW = false;

    vector<vector<double>> matrizBase = algoritmoBase(aristas, n, cicloNegBase);
    vector<vector<double>> matrizFW = algoritmoFloydWarshall(aristas, n, cicloNegFW);

    cout << "=== " << archivo << " (n=" << n << ") ===\n";
    cout << "  Ciclo negativo (algoritmoBase):          " << (cicloNegBase ? "si" : "no") << "\n";
    cout << "  Ciclo negativo (algoritmoFloydWarshall): " << (cicloNegFW ? "si" : "no") << "\n";

    if (cicloNegBase != cicloNegFW) {
        cout << "  ERROR: los flags de ciclo negativo difieren\n\n";
        return false;
    }

    if (cicloNegBase || cicloNegFW) {
        cout << "  Ciclo negativo detectado: las matrices puntuales pueden diferir (esperado)\n\n";
        return true;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (abs(matrizBase[i][j] - matrizFW[i][j]) > 1e-9) {
                cout << "  ERROR: matrices difieren en [" << i << "][" << j << "]: base="
                     << matrizBase[i][j] << ", FW=" << matrizFW[i][j] << "\n\n";
                return false;
            }
        }
    }
    cout << "  Matrices coinciden\n";

    auto [uB, vB, dB] = encontrarParMasCercano(matrizBase);
    auto [uF, vF, dF] = encontrarParMasCercano(matrizFW);
    cout << "  Par mas cercano (algoritmoBase):          (" << uB << ", " << vB << ") = " << dB << "\n";
    cout << "  Par mas cercano (algoritmoFloydWarshall): (" << uF << ", " << vF << ") = " << dF << "\n";

    if (uB != uF || vB != vF || abs(dB - dF) > 1e-9) {
        cout << "  ERROR: los pares difieren\n\n";
        return false;
    }

    // Verificacion contra valores esperados (calculados a mano, ver informe)
    if (archivo == "test.mtx") {
        assert(n == 4);
        assert(uB == 1 && vB == 3 && abs(dB - 1.5) < 1e-9);
        cout << "  Valor esperado verificado: par (1, 3) = 1.5\n";
    } else if (archivo == "test_neg.mtx") {
        assert(n == 5);
        assert(uB == 1 && vB == 2 && abs(dB - (-5.0)) < 1e-9);
        cout << "  Valor esperado verificado: par (1, 2) = -5\n";
    } else if (archivo == "test_grande.mtx") {
        cout << "  Instancia aleatoria (n=" << n << "): sin valor esperado hardcodeado\n";
    }

    cout << "\n";
    return true;
}

int main(){
    bool todoOK = true;
    todoOK &= verificar("test.mtx");
    todoOK &= verificar("test_neg.mtx");
    todoOK &= verificar("test_negcycle.mtx");
    // todoOK &= verificar("test_grande.mtx");  // comentado: n=1000, ~5 min
    cout << (todoOK ? "TODOS LOS TESTS PASARON" : "ALGUN TEST FALLO") << "\n";
    return todoOK ? 0 : 1;
}