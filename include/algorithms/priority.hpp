#pragma once

#include "core/scheduler.hpp"
#include <queue>
#include <vector>
#include <algorithm>

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
        ready_queue_.push_back(process);
        sort_queue();
    }

    std::optional<std::shared_ptr<Process>> get_next_process() override {
        if (ready_queue_.empty()) {
            return std::nullopt;
        }
        auto next = ready_queue_.front();
        ready_queue_.erase(ready_queue_.begin());
        next->set_state(Process::ProcessState::RUNNING);
        return next;
    }

    void preempt_process(std::shared_ptr<Process> current_process) override {
        if (current_process && current_process->remaining_time() > 0) {
            current_process->set_state(Process::ProcessState::READY);
            ready_queue_.push_back(current_process);
            sort_queue();
        }
    }

    bool needs_preemption(std::shared_ptr<Process> current_process, int) override {
        if (!preemptive_ || ready_queue_.empty() || !current_process) {
            return false;
        }
        return ready_queue_.front()->priority() < current_process->priority();
    }

    std::string name() const override {
        return preemptive_ ? "Preemptive Priority" : "Non-preemptive Priority";
    }

    size_t ready_queue_size() const {
        return ready_queue_.size();
    }

private:
    void sort_queue() {
        std::sort(ready_queue_.begin(), ready_queue_.end(),
            [](const auto& a, const auto& b) {
                return a->priority() < b->priority();
            });
    }

    bool preemptive_;
    std::vector<std::shared_ptr<Process>> ready_queue_;
};

} // namespace cpu_scheduler 