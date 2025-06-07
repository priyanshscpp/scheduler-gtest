#pragma once

#include "core/scheduler.hpp"
#include <queue>
#include <vector>
#include <algorithm>

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

    bool needs_preemption(std::shared_ptr<Process>, int) override {
        return false;  // Non-preemptive SJF
    }

    std::string name() const override {
        return "Shortest Job First";
    }

    size_t ready_queue_size() const {
        return ready_queue_.size();
    }

private:
    void sort_queue() {
        std::sort(ready_queue_.begin(), ready_queue_.end(),
            [](const auto& a, const auto& b) {
                return a->remaining_time() < b->remaining_time();
            });
    }

    std::vector<std::shared_ptr<Process>> ready_queue_;
};

} // namespace cpu_scheduler 