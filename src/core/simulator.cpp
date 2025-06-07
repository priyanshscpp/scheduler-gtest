#include "core/simulator.hpp"
#include <algorithm>
#include <iostream>

namespace cpu_scheduler {

void Simulator::add_process(int arrival_time, int burst_time, int priority) {
    processes_.push_back(std::make_shared<Process>(
        next_pid_++, arrival_time, burst_time, priority
    ));
}

SimulationStats Simulator::run() {
    SimulationStats stats;
    int current_time = 0;
    std::shared_ptr<Process> current_process = nullptr;

    while (!is_simulation_complete()) {
        // Add newly arrived processes
        add_arrived_processes(current_time);

        // Check if we need to preempt current process
        if (current_process && scheduler_->needs_preemption(current_process, current_time)) {
            scheduler_->preempt_process(current_process);
            current_process = nullptr;
            stats.total_context_switches++;
            current_time += context_switch_overhead_;
        }

        // Get next process if none running
        if (!current_process) {
            auto next = scheduler_->get_next_process();
            if (next) {
                current_process = *next;
                stats.total_context_switches++;
                current_time += context_switch_overhead_;
            }
        }

        // Execute current process
        if (current_process) {
            current_process->decrement_remaining_time();
            if (current_process->remaining_time() == 0) {
                stats.completed_processes++;
                int turnaround = current_time - current_process->arrival_time();
                int waiting = turnaround - current_process->burst_time();
                stats.avg_turnaround_time += turnaround;
                stats.avg_waiting_time += waiting;
                current_process = nullptr;
            }
        }

        current_time++;
    }

    // Calculate averages
    if (stats.completed_processes > 0) {
        stats.avg_turnaround_time /= stats.completed_processes;
        stats.avg_waiting_time /= stats.completed_processes;
    }

    return stats;
}

bool Simulator::is_simulation_complete() const {
    return std::all_of(processes_.begin(), processes_.end(),
        [](const auto& p) { return p->remaining_time() == 0; });
}

void Simulator::add_arrived_processes(int current_time) {
    for (const auto& process : processes_) {
        if (process->arrival_time() == current_time) {
            scheduler_->add_process(process);
        }
    }
}

} // namespace cpu_scheduler 