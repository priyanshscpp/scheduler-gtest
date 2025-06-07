#pragma once

#include "core/scheduler.hpp"
#include <queue>
#include <vector>

namespace cpu_scheduler {

/**
 * @brief Shortest Job First (SJF) scheduling algorithm implementation
 * 
 * This scheduler implements the SJF algorithm, which selects the process with
 * the shortest burst time to execute next. This implementation is non-preemptive.
 */
class SJFScheduler : public Scheduler {
public:
    SJFScheduler() = default;

    void add_process(std::shared_ptr<Process> process) override {
        processes_[process->pid()] = process;
        process->set_state(Process::ProcessState::READY);
        ready_queue_.push(process);
    }

    std::optional<std::shared_ptr<Process>> get_next_process() override {
        if (ready_queue_.empty()) {
            return std::nullopt;
        }

        // Find process with shortest burst time
        auto shortest = ready_queue_.top();
        ready_queue_.pop();
        
        shortest->set_state(Process::ProcessState::RUNNING);
        return shortest;
    }

    void preempt_process(std::shared_ptr<Process> current_process) override {
        // SJF is non-preemptive in this implementation
        (void)current_process;
    }

    bool needs_preemption(std::shared_ptr<Process> current_process, int current_time) override {
        // Only preempt when process is complete
        return !current_process || current_process->remaining_time() <= 0;
    }

    std::string name() const override {
        return "Shortest Job First";
    }

    size_t ready_queue_size() const {
        return ready_queue_.size();
    }

private:
    // Custom comparator for priority queue
    struct SJFComparator {
        bool operator()(const std::shared_ptr<Process>& a, const std::shared_ptr<Process>& b) const {
            return a->burst_time() > b->burst_time();
        }
    };

    std::priority_queue<
        std::shared_ptr<Process>,
        std::vector<std::shared_ptr<Process>>,
        SJFComparator
    > ready_queue_;
};

} // namespace cpu_scheduler 