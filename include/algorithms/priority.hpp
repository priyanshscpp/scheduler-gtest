#pragma once

#include "core/scheduler.hpp"
#include <queue>
#include <vector>

namespace cpu_scheduler {

/**
 * @brief Priority scheduling algorithm implementation
 * 
 * This scheduler implements the Priority scheduling algorithm, which selects the process
 * with the highest priority to execute next. Lower priority number means higher priority.
 * This implementation is preemptive.
 */
class PriorityScheduler : public Scheduler {
public:
    explicit PriorityScheduler(bool preemptive = true)
        : preemptive_(preemptive) {}

    void add_process(std::shared_ptr<Process> process) override {
        processes_[process->pid()] = process;
        process->set_state(Process::ProcessState::READY);
        ready_queue_.push(process);
    }

    std::optional<std::shared_ptr<Process>> get_next_process() override {
        if (ready_queue_.empty()) {
            return std::nullopt;
        }

        auto highest_priority = ready_queue_.top();
        ready_queue_.pop();
        
        highest_priority->set_state(Process::ProcessState::RUNNING);
        return highest_priority;
    }

    void preempt_process(std::shared_ptr<Process> current_process) override {
        if (current_process && current_process->remaining_time() > 0) {
            current_process->set_state(Process::ProcessState::READY);
            ready_queue_.push(current_process);
        }
    }

    bool needs_preemption(std::shared_ptr<Process> current_process, int current_time) override {
        if (!current_process || current_process->remaining_time() <= 0) {
            return true;
        }

        if (!preemptive_) {
            return false;
        }

        // Check if there's a higher priority process in the queue
        if (!ready_queue_.empty()) {
            auto top_priority = ready_queue_.top()->priority();
            return top_priority < current_process->priority();
        }

        return false;
    }

    std::string name() const override {
        return std::string("Priority Scheduling (") + 
               (preemptive_ ? "Preemptive" : "Non-preemptive") + ")";
    }

    size_t ready_queue_size() const {
        return ready_queue_.size();
    }

private:
    bool preemptive_;

    // Custom comparator for priority queue (lower number = higher priority)
    struct PriorityComparator {
        bool operator()(const std::shared_ptr<Process>& a, const std::shared_ptr<Process>& b) const {
            return a->priority() > b->priority();
        }
    };

    std::priority_queue<
        std::shared_ptr<Process>,
        std::vector<std::shared_ptr<Process>>,
        PriorityComparator
    > ready_queue_;
};

} // namespace cpu_scheduler 