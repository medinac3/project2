#!/bin/bash

PROGRAM="main"
SOURCE="main.c"
RUNS=60000

gcc -pthread -o "$PROGRAM" "$SOURCE"

echo "Running single-thread mode $RUNS times..."
start_single=$(date +%s%N)
for ((i=1; i<=RUNS; i++)); do
    ./"$PROGRAM" 1 > /dev/null
done
end_single=$(date +%s%N)

single_ns=$((end_single - start_single))
single_ms=$((single_ns / 1000000))

echo "Running multithread mode $RUNS times..."
start_multi=$(date +%s%N)
for ((i=1; i<=RUNS; i++)); do
    ./"$PROGRAM" 2 > /dev/null
done
end_multi=$(date +%s%N)

multi_ns=$((end_multi - start_multi))
multi_ms=$((multi_ns / 1000000))

echo
echo "RESULTS"
echo "Single-thread total time: ${single_ms} ms"
echo "Multithread total time : ${multi_ms} ms"

if [ "$single_ns" -lt "$multi_ns" ]; then
    echo "Single-thread mode was faster."
elif [ "$multi_ns" -lt "$single_ns" ]; then
    echo "Multithread mode was faster."
else
    echo "Both modes took the same time."
fi