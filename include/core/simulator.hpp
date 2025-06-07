#pragma once

#include "core/scheduler.hpp"
#include <memory>
#include <vector>
#include <sstream>
#include <iomanip>

namespace cpu_scheduler {

/**
 * @brief Statistics collected during simulation
 */
struct SimulationStats {
    double avg_waiting_time{0.0};
    double avg_turnaround_time{0.0};
    int total_context_switches{0};
    int completed_processes{0};

    std::string to_string() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2)
           << "Average Waiting Time: " << avg_waiting_time << "ms\n"
           << "Average Turnaround Time: " << avg_turnaround_time << "ms\n"
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
    Simulator(std::unique_ptr<Scheduler> scheduler, int context_switch_overhead = 0)
        : scheduler_(std::move(scheduler)), 
          context_switch_overhead_(context_switch_overhead),
          next_pid_(1) {}

    /**
     * @brief Add a new process to the simulation
     */
    void add_process(int arrival_time, int burst_time, int priority = 0);

    /**
     * @brief Run the simulation until completion
     * @return Statistics from the simulation run
     */
    SimulationStats run();

    /**
     * @brief Get the current simulation time
     */
    int current_time() const;

    /**
     * @brief Get the context switch overhead
     */
    int context_switch_overhead() const;

private:
    bool is_simulation_complete() const;
    void add_arrived_processes(int current_time);

    std::unique_ptr<Scheduler> scheduler_;
    std::vector<std::shared_ptr<Process>> processes_;
    int context_switch_overhead_;
    int current_time_{0};
    int next_pid_;
    bool in_context_switch_{false};
    int context_switch_time_remaining_{0};
    SimulationStats stats_;
};

} // namespace cpu_scheduler 