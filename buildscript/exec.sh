#!/usr/bin/env bash
set -e

cd "$(dirname "$0")"

if [ -e "../testcases/$1" ]; then
    mkdir -p ../result
    ./build/tspbs < "../testcases/$1" > ../result/tspbsResult.txt
    python ../src/plot.py ../result/tspbsResult.txt -o ../result/tour.png
else
    echo "file not found: ../testcases/$1" >&2
    exit 1
fi
