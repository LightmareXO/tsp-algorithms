#!/usr/bin/env bash
mkdir -p build
g++ ../src/tspbs.cpp -o build/tspbs -O3 -std=c++17