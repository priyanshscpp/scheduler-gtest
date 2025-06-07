#pragma once

#include "core/scheduler.hpp"
#include <queue>

namespace cpu_scheduler {

/**
 * @brief First Come First Serve (FCFS) scheduling algorithm implementation
 * 
 * This scheduler implements the FCFS algorithm, which is the simplest scheduling
 * algorithm. Processes are executed in the order they arrive, with no preemption.
 */
class FCFSScheduler : public Scheduler {
public:
    FCFSScheduler() = default;

    void add_process(std::shared_ptr<Process> process) override {
        processes_[process->pid()] = process;
        process->set_state(Process::ProcessState::READY);
        ready_queue_.push(process);
    }

    std::optional<std::shared_ptr<Process>> get_next_process() override {
        if (ready_queue_.empty()) {
            return std::nullopt;
        }

        auto next_process = ready_queue_.front();
        ready_queue_.pop();
        
        next_process->set_state(Process::ProcessState::RUNNING);
        return next_process;
    }

    void preempt_process(std::shared_ptr<Process> current_process) override {
        // FCFS is non-preemptive, so this is a no-op
        (void)current_process;
    }

    bool needs_preemption(std::shared_ptr<Process> current_process, int current_time) override {
        // FCFS is non-preemptive, only preempt when process is complete
        return !current_process || current_process->remaining_time() <= 0;
    }

    std::string name() const override {
        return "First Come First Serve";
    }

    size_t ready_queue_size() const {
        return ready_queue_.size();
    }

private:
    std::queue<std::shared_ptr<Process>> ready_queue_;
};

} // namespace cpu_scheduler 