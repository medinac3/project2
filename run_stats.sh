#!/bin/bash

OUTPUT_FILE="timing_results.txt"

> $OUTPUT_FILE

echo "Starting benchmarks..." | tee -a $OUTPUT_FILE

for arg in 1 2; do
    echo "---------------------------------------" >> $OUTPUT_FILE
    echo "Running 'main' with argument: $arg" >> $OUTPUT_FILE
    echo "---------------------------------------" >> $OUTPUT_FILE

    for i in {1..30}; do
        echo "Run #$i:" >> $OUTPUT_FILE
        
        { time ./main $arg ; } 2>> $OUTPUT_FILE
        
        echo "" >> $OUTPUT_FILE
    done
done

echo "Benchmarks complete. Results saved to $OUTPUT_FILE."
