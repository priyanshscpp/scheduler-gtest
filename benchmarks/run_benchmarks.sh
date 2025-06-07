#!/bin/bash

BINARY="../build/cpu-scheduler"

echo "Running CPU Scheduler Benchmarks"
echo "==============================="

for scenario in mixed_load heavy_load io_intensive; do
    echo -e "\nTesting $scenario scenario:"
    echo "-----------------------------"
    
    # Test with different schedulers
    for algo in rr fcfs sjf prio; do
        echo -e "\n$algo scheduler:"
        if [ "$algo" = "rr" ]; then
            $BINARY -a $algo -q 2 -w scenarios.json --scenario $scenario
        else
            $BINARY -a $algo -w scenarios.json --scenario $scenario
        fi
    done
done 