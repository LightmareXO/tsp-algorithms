#!/usr/bin/env bash
set -e

cd "$(dirname "$0")"

mkdir -p build
g++ ../src/tspbs.cpp -o build/tspbs -O3 -std=c++17
