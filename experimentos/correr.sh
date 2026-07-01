#!/bin/bash
# Opción B: base chico (25-200), FW grande (25-500)
# 32 corridas por n. Densidad 0.3, semilla 42.
# No correr hasta confirmar: ./correr.sh

set -e
cd "$(dirname "$0")"

# Compilar con -O0 (requerido por uhr)
echo "Compilando uhr_base..."
g++ -std=c++11 -O0 -o uhr_base uhr_base.cpp

echo "Compilando uhr_fw..."
g++ -std=c++11 -O0 -o uhr_fw uhr_fw.cpp

# Limpiar ejecuciones anteriores
rm -f resultado_base.csv resultado_fw.csv temp.mtx

# Opción B
echo "Corriendo uhr_base (n=25..200, step=25, 32 corridas)..."
./uhr_base resultado_base.csv 32 25 200 25

echo "Corriendo uhr_fw (n=25..500, step=25, 32 corridas)..."
./uhr_fw resultado_fw.csv 32 25 500 25

echo "Listo. CSVs: resultado_base.csv, resultado_fw.csv"
