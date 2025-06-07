# CPU Scheduler Benchmarks

This directory contains benchmark scenarios to evaluate scheduler performance.

## Running Benchmarks
```bash
./run_benchmarks.sh
```

## Scenarios

1. **Mixed Load**
   - 10 processes with varying burst times and priorities
   - Mix of CPU-bound and I/O-bound processes
   - Arrival pattern: mix of bulk and staggered arrivals

2. **Heavy Load**
   - 50 processes with long burst times
   - All CPU-bound processes
   - Tests scheduler efficiency under stress

3. **I/O Intensive**
   - 20 processes with short burst times
   - Frequent context switches
   - Tests context switch overhead handling

## Metrics
- Average waiting time
- Average turnaround time
- CPU utilization
- Context switches per second
- Response time 