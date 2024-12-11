#!/bin/bash

process_output="processes_created_destroyed.txt"

echo "Process (Created/Destroyed) - Timestamp" >"$process_output"

prev_process_list=$(mktemp)
cur_process_list=$(mktemp)

ps -e -o comm= | grep -v "kworker" >"$prev_process_list"

for i in $(seq 1 3600); do
    sleep 1
    timestamp=${i}

    ps -e -o comm= | grep -v "kworker" >"$cur_process_list"
    created_processes=$(comm -13 "$prev_process_list" "$cur_process_list" 2>/dev/null)
    destroyed_processes=$(comm -23 "$prev_process_list" "$cur_process_list" 2>/dev/null)

    echo -e "*************************************" >>"$process_output"
    echo "------Created-$timestamp-----------" >>"$process_output"
    if [[ -n "$created_processes" ]]; then
        echo "$created_processes " >>"$process_output"
    fi
    echo -e "\n------Destroyed-$timestamp---------" >>"$process_output"
    if [[ -n "$destroyed_processes" ]]; then
        echo "$destroyed_processes " >>"$process_output"
    fi
    echo -e "*************************************\n" >>"$process_output"
    mv "$cur_process_list" "$prev_process_list"
done

rm "$prev_process_list"
