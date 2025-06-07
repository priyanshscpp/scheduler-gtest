#pragma once

#include "scheduler.hpp"
#include "../utils/visualization.hpp"
#include <memory>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>

namespace cpu_scheduler {

/**
 * @brief Statistics collected during simulation
 */
struct SimulationStats {
    double avg_waiting_time{0.0};
    double avg_turnaround_time{0.0};
    double avg_response_time{0.0};
    int total_context_switches{0};
    int completed_processes{0};

    std::string to_string() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2)
           << "Average Waiting Time: " << avg_waiting_time << "ms\n"
           << "Average Turnaround Time: " << avg_turnaround_time << "ms\n"
           << "Average Response Time: " << avg_response_time << "ms\n"
           << "Total Context Switches: " << total_context_switches << "\n"
           << "Completed Processes: " << completed_processes;
        return ss.str();
    }
};

/**
 * @brief Main simulator class that manages the scheduling simulation
 */
class Simulator {
public:
    explicit Simulator(std::unique_ptr<Scheduler> scheduler, int context_switch_overhead = 0)
        : scheduler_(std::move(scheduler)), 
          context_switch_overhead_(context_switch_overhead),
          current_time_(0) {}

    /**
     * @brief Add a new process to the simulation
     */
    void add_process(int arrival_time, int burst_time, int priority = 0) {
        auto process = std::make_shared<Process>(
            next_pid_++, arrival_time, burst_time, priority
        );
        processes_.push_back(process);
    }

    /**
     * @brief Run the simulation until completion
     * @return Statistics from the simulation run
     */
    SimulationStats run() {
        std::vector<std::shared_ptr<Process>> arrived_processes;
        size_t next_process_idx = 0;
        std::shared_ptr<Process> current_process;
        SimulationStats stats;
        SchedulerVisualizer visualizer;

        while (true) {
            // Check for newly arrived processes
            while (next_process_idx < processes_.size() && 
                   processes_[next_process_idx]->arrival_time() <= current_time_) {
                scheduler_->add_process(processes_[next_process_idx]);
                next_process_idx++;
            }

            // Handle context switch overhead
            if (in_context_switch_) {
                if (context_switch_time_remaining_ > 0) {
                    context_switch_time_remaining_--;
                    current_time_++;
                    continue;
                }
                in_context_switch_ = false;
            }

            // Check if current process needs to be preempted
            if (current_process && scheduler_->needs_preemption(current_process, current_time_)) {
                visualizer.record_event(current_time_, current_process->pid(), "PREEMPT");
                scheduler_->preempt_process(current_process);
                start_context_switch();
                current_process = nullptr;
                continue;
            }

            // Get next process if CPU is idle
            if (!current_process) {
                auto next_process = scheduler_->get_next_process();
                if (!next_process) {
                    if (next_process_idx >= processes_.size()) {
                        break; // Simulation complete
                    }
                    current_time_++;
                    continue;
                }
                current_process = *next_process;
                visualizer.record_event(current_time_, current_process->pid(), "START");
                start_context_switch();
                continue;
            }

            // Execute current process
            current_process->decrement_remaining_time();
            
            // Check if process has completed
            if (current_process->remaining_time() <= 0) {
                visualizer.record_event(current_time_, current_process->pid(), "COMPLETE");
                update_stats(current_process, stats);
                current_process = nullptr;
            }

            current_time_++;
        }

        finalize_stats(stats);

        // Generate and print visualization
        std::cout << visualizer.generate_timeline() << std::endl;
        std::cout << visualizer.generate_metrics(stats) << std::endl;

        return stats;
    }

    /**
     * @brief Get the current simulation time
     */
    int current_time() const { return current_time_; }

    /**
     * @brief Get the context switch overhead
     */
    int context_switch_overhead() const { return context_switch_overhead_; }

private:
    void start_context_switch() {
        if (context_switch_overhead_ > 0) {
            in_context_switch_ = true;
            context_switch_time_remaining_ = context_switch_overhead_;
            stats_.total_context_switches++;
        }
    }

    void update_stats(std::shared_ptr<Process> completed_process, SimulationStats& stats) {
        int turnaround_time = current_time_ - completed_process->arrival_time();
        int waiting_time = turnaround_time - completed_process->burst_time();
        
        stats.avg_waiting_time += waiting_time;
        stats.avg_turnaround_time += turnaround_time;
        stats.completed_processes++;
    }

    void finalize_stats(SimulationStats& stats) {
        if (stats.completed_processes > 0) {
            stats.avg_waiting_time /= stats.completed_processes;
            stats.avg_turnaround_time /= stats.completed_processes;
            stats.avg_response_time /= stats.completed_processes;
        }
    }

    std::unique_ptr<Scheduler> scheduler_;
    std::vector<std::shared_ptr<Process>> processes_;
    int context_switch_overhead_;
    int current_time_{0};
    int next_pid_{1};
    bool in_context_switch_{false};
    int context_switch_time_remaining_{0};
    SimulationStats stats_;
};

} // namespace cpu_scheduler 