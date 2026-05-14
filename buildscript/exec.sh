#!/usr/bin/env bash

# ex) ./exec.sh tspbs eil76.tsp 
set -e

cd "$(dirname "$0")"

if [ $# -lt 2 ]; then
    echo "usage: $0 <executable> <testcase.tsp>" >&2
    exit 1
fi

exe="./build/$1"
testcase="../testcases/$2"
name="${1%.exe}"

if [ ! -e "$exe" ]; then
    echo "executable not found: $exe" >&2
    exit 1
fi

if [ ! -e "$testcase" ]; then
    echo "file not found: $testcase" >&2
    exit 1
fi

mkdir -p ../result

result="../result/${name}Result.txt"
tour="../result/${name}Tour.png"

"$exe" < "$testcase" > "$result"
python ../src/plot.py "$result" -o "$tour"

score="$(head -n 1 "$result")"
echo "$name: score=$score, image=$tour"
