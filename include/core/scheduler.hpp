#pragma once

#include <memory>
#include <queue>
#include <vector>
#include <chrono>
#include <optional>
#include <string>
#include <unordered_map>

namespace cpu_scheduler {

/**
 * @brief Process Control Block (PCB) representing a process in the system
 */
class Process {
public:
    Process(int pid, int arrival_time, int burst_time, int priority = 0)
        : pid_(pid), arrival_time_(arrival_time), burst_time_(burst_time),
          remaining_time_(burst_time), priority_(priority), state_(ProcessState::NEW) {}

    enum class ProcessState {
        NEW,
        READY,
        RUNNING,
        WAITING,
        TERMINATED,
        ZOMBIE
    };

    // Getters
    int pid() const { return pid_; }
    int arrival_time() const { return arrival_time_; }
    int burst_time() const { return burst_time_; }
    int remaining_time() const { return remaining_time_; }
    int priority() const { return priority_; }
    ProcessState state() const { return state_; }
    
    // Setters
    void set_state(ProcessState state) { state_ = state; }
    void set_remaining_time(int time) { remaining_time_ = time; }
    void decrement_remaining_time() { if (remaining_time_ > 0) remaining_time_--; }

private:
    int pid_;
    int arrival_time_;
    int burst_time_;
    int remaining_time_;
    int priority_;
    ProcessState state_;
};

/**
 * @brief Abstract base class for all scheduling algorithms
 */
class Scheduler {
public:
    virtual ~Scheduler() = default;

    /**
     * @brief Add a new process to be scheduled
     * @param process Shared pointer to the process
     */
    virtual void add_process(std::shared_ptr<Process> process) = 0;

    /**
     * @brief Get the next process to be scheduled
     * @return Optional containing the next process, or empty if none available
     */
    virtual std::optional<std::shared_ptr<Process>> get_next_process() = 0;

    /**
     * @brief Preempt the currently running process
     * @param current_process The process to preempt
     */
    virtual void preempt_process(std::shared_ptr<Process> current_process) = 0;

    /**
     * @brief Check if preemption is needed for the current process
     * @param current_process The currently running process
     * @param current_time Current simulation time
     * @return True if preemption is needed, false otherwise
     */
    virtual bool needs_preemption(std::shared_ptr<Process> current_process, int current_time) = 0;

    /**
     * @brief Get the name of the scheduling algorithm
     * @return String containing the algorithm name
     */
    virtual std::string name() const = 0;

protected:
    std::unordered_map<int, std::shared_ptr<Process>> processes_;
};

} // namespace cpu_scheduler 