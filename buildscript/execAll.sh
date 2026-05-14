#!/usr/bin/env bash
#codex君に任せました

# ex) ./exec.sh eil76.tsp 
set -e

cd "$(dirname "$0")"

if [ $# -lt 1 ]; then
    echo "usage: $0 <testcase.tsp>" >&2
    exit 1
fi

testcase="../testcases/$1"

if [ ! -e "$testcase" ]; then
    echo "file not found: $testcase" >&2
    exit 1
fi

mkdir -p ../result

run_one() {
    local exe="$1"
    local name
    name="$(basename "$exe")"
    name="${name%.exe}"

    local result="../result/${name}Result.txt"
    local tour="../result/${name}Tour.png"

    "$exe" < "$testcase" > "$result"
    python ../src/plot.py "$result" -o "$tour"

    local score
    score="$(sed -n '3p' "$result")"
    echo "$name: score=$score, image=$tour"
}

found=0
pids=()

for exe in build/*; do
    if [ -f "$exe" ]; then
        found=1
        run_one "$exe" &
        pids+=("$!")
    fi
done

if [ "$found" -eq 0 ]; then
    echo "no executable files found in build/. Run buildAll.sh first." >&2
    exit 1
fi

for pid in "${pids[@]}"; do
    wait "$pid"
done
