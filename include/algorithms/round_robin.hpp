#pragma once

#include "core/scheduler.hpp"
#include <queue>

namespace cpu_scheduler {

/**
 * @brief Round Robin scheduling algorithm implementation
 * 
 * This scheduler implements the Round Robin algorithm with a configurable time quantum.
 * Processes are executed in FIFO order, with each process getting a maximum time slice
 * equal to the quantum before being preempted.
 */
class RoundRobinScheduler : public Scheduler {
public:
    explicit RoundRobinScheduler(int quantum) 
        : quantum_(quantum), current_time_slice_(0) {}

    void add_process(std::shared_ptr<Process> process) override {
        processes_[process->pid()] = process;
        process->set_state(Process::ProcessState::READY);
        ready_queue_.push(process);
    }

    std::optional<std::shared_ptr<Process>> get_next_process() override {
        if (ready_queue_.empty()) {
            return std::nullopt;
        }

        auto next = ready_queue_.front();
        ready_queue_.pop();
        next->set_state(Process::ProcessState::RUNNING);
        current_time_slice_ = 0;
        return next;
    }

    void preempt_process(std::shared_ptr<Process> current_process) override {
        if (current_process && current_process->remaining_time() > 0) {
            current_process->set_state(Process::ProcessState::READY);
            ready_queue_.push(current_process);
        }
        current_time_slice_ = 0;
    }

    bool needs_preemption(std::shared_ptr<Process> current_process, int current_time) override {
        if (!current_process || current_process->remaining_time() <= 0) {
            return true;
        }

        current_time_slice_++;
        return current_time_slice_ >= quantum_;
    }

    std::string name() const override {
        return "Round Robin (Q=" + std::to_string(quantum_) + ")";
    }

    // Additional RR-specific methods
    int quantum() const { return quantum_; }
    int current_time_slice() const { return current_time_slice_; }
    size_t ready_queue_size() const { return ready_queue_.size(); }

private:
    int quantum_;
    int current_time_slice_;
    std::queue<std::shared_ptr<Process>> ready_queue_;
};

} // namespace cpu_scheduler 