/** uhr: time performance tester for algoritmoFloydWarshall
 * Migrated functions from tarea2.cpp */

#include <cstdint>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <tuple>

#include "uhr/utils.cpp"

using namespace std;

// ============ Funciones migradas de tarea2.cpp ============

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

vector<vector<double>> listaAMatriz(const vector<Edge>& aristas, int nodos) {
    vector<vector<double>> matriz(nodos, vector<double>(nodos, INF));
    for (const Edge& a : aristas) {
        matriz[a.u][a.v] = a.peso;
    }
    for (int i = 0; i < nodos; i++) matriz[i][i] = 0;
    return matriz;
}

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

void generarGrafo(int n, double densidad, unsigned semilla, const string& archivo) {
    srand(semilla);

    ofstream f(archivo);
    if (!f.is_open()) {
        cerr << "Error: no se pudo crear '" << archivo << "'\n";
        exit(1);
    }

    f << "%%MatrixMarket matrix coordinate real general\n";

    vector<tuple<int, int, int>> aristas;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (i != j && (rand() / (double)RAND_MAX) < densidad) {
                int peso = 1 + (rand() % 100);
                aristas.push_back({i, j, peso});
            }
        }
    }

    f << n << " " << n << " " << aristas.size() << "\n";
    for (auto& [u, v, p] : aristas) {
        f << u << " " << v << " " << p << "\n";
    }
    f.close();
}

// ============ uhr framework ============

int main(int argc, char *argv[])
{
    std::int64_t runs, lower, upper, step;
    validate_input(argc, argv, runs, lower, upper, step);

    std::int64_t n, i, executed_runs;
    std::int64_t total_runs_additive = runs * (((upper - lower) / step) + 1);
    std::vector<double> times(runs);
    std::vector<double> q;
    double mean_time, time_stdev, dev;
    auto begin_time = std::chrono::high_resolution_clock::now();
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::nano> elapsed_time = end_time - begin_time;

    std::ofstream time_data;
    time_data.open(argv[1]);
    time_data << "n,t_mean,t_stdev,t_Q0,t_Q1,t_Q2,t_Q3,t_Q4" << std::endl;

    std::cerr << "\033[0;36mRunning tests...\033[0m" << std::endl << std::endl;
    executed_runs = 0;
    for (n = lower; n <= upper; n += step) {
        mean_time = 0;
        time_stdev = 0;

        // Setup: generate graph once per n (I/O outside timing loop)
        generarGrafo(n, 0.3, 42, "temp.mtx");
        auto [nn, aristas] = crearListaDeAristas("temp.mtx");
        bool dummy = false;

        // Run to compute elapsed time (only the algorithm)
        for (i = 0; i < runs; i++) {
            display_progress(++executed_runs, total_runs_additive);

            begin_time = std::chrono::high_resolution_clock::now();
            algoritmoFloydWarshall(aristas, nn, dummy);
            end_time = std::chrono::high_resolution_clock::now();

            elapsed_time = end_time - begin_time;
            times[i] = elapsed_time.count();
            mean_time += times[i];
        }

        // Compute statistics
        mean_time /= runs;

        for (i = 0; i < runs; i++) {
            dev = times[i] - mean_time;
            time_stdev += dev * dev;
        }

        time_stdev /= runs - 1;
        time_stdev = std::sqrt(time_stdev);

        quartiles(times, q);

        time_data << n << "," << mean_time << "," << time_stdev << ",";
        time_data << q[0] << "," << q[1] << "," << q[2] << "," << q[3] << "," << q[4] << std::endl;
    }

    std::cerr << std::endl << std::endl;
    std::cerr << "\033[1;32mDone!\033[0m" << std::endl;

    time_data.close();

    return 0;
}
