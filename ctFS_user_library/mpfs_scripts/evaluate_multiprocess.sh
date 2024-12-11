#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <max_number_of_processes> <repeats_per_process_count>"
    exit 1
fi

MAX_PROCESSES=$1
REPEATS=$2
SCRIPT_NAME="./run_multiprocess.sh"
RESULTS_FILE="experiment_results.csv"

header="Process Count,Average Time (seconds),Average Time (nano seconds)"
for ((i = 1; i <= $REPEATS; i++)); do
    header+=",Repeat $i"
done
echo $header >$RESULTS_FILE

for ((p = 1; p <= $MAX_PROCESSES; p++)); do
    total_time=0
    echo "Running tests for $p processes..."

    for ((r = 1; r <= $REPEATS; r++)); do
        echo "Repeat $r for $p processes..."

        output=$(./$SCRIPT_NAME $p)

        time_taken_ns=$(echo "$output" | grep "Time taken:" | awk '{print $3}')
        total_time=$(echo "$total_time + $time_taken_ns" | bc)
        row+=",$time_taken_ns"
    done

    avg_time_ns=$(echo "scale=9; $total_time / $REPEATS" | bc)
    avg_time_s=$(echo "scale=9; $avg_time_ns/1000000000" | bc)
    row="$p, $avg_time_s,$avg_time_ns $row"

    echo $row >>$RESULTS_FILE
    row=""
done
