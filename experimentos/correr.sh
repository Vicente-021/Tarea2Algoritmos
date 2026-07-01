#!/bin/bash
# Benchmarking con 4 configs (sparse/dense × positivos/negativos).
# Cada binario corre las 4 configs y produce 4 CSVs (uno por config).
# 32 corridas por n. Misma semilla 42 para reproducibilidad.
# Opciones B-3: un binario por algoritmo, 4 configs internas.
# No correr hasta confirmar: ./correr.sh

set -e
cd "$(dirname "$0")"

# Compilar con -O0 (requerido por uhr)
echo "Compilando uhr_base..."
g++ -std=c++17 -O0 -o uhr_base uhr_base.cpp

echo "Compilando uhr_fw..."
g++ -std=c++17 -O0 -o uhr_fw uhr_fw.cpp

# Limpiar ejecuciones anteriores
rm -f resultado_base_*.csv resultado_fw_*.csv temp.mtx

# Correr benchmarks (cada uno produce 4 CSVs: sparse_pos, sparse_neg, dense_pos, dense_neg)
echo "Corriendo uhr_base (n=25..200, step=25, 32 corridas, 4 configs)..."
./uhr_base resultado_base 32 25 200 25

echo "Corriendo uhr_fw (n=25..500, step=25, 32 corridas, 4 configs)..."
./uhr_fw resultado_fw 32 25 500 25

echo "Listo. CSVs generados:"
ls -1 resultado_base_*.csv resultado_fw_*.csv
