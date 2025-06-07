#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <memory>
#include <map>
#include "core/process.hpp"

namespace cpu_scheduler {

struct TimelineEvent {
    int time;
    int pid;
    std::string event_type; // "START", "PREEMPT", "COMPLETE"
};

class SchedulerVisualizer {
public:
    void record_event(int time, int pid, const std::string& event_type) {
        events_.push_back({time, pid, event_type});
    }

    std::string generate_timeline() const {
        if (events_.empty()) {
            return "No events recorded.";
        }

        std::stringstream ss;
        ss << "\nScheduling Timeline:\n";
        ss << "================\n\n";

        // Generate time scale
        ss << "Time: ";
        for (int t = 0; t <= events_.back().time; t++) {
            ss << std::setw(3) << t;
        }
        ss << "\n";

        // Generate process execution bars
        std::map<int, std::string> process_lines;
        
        // Initialize process lines
        for (const auto& event : events_) {
            if (process_lines.find(event.pid) == process_lines.end()) {
                process_lines[event.pid] = std::string(events_.back().time + 1, ' ');
            }
        }

        // Fill in process execution
        for (size_t i = 0; i < events_.size(); i++) {
            const auto& event = events_[i];
            auto& line = process_lines[event.pid];

            if (event.event_type == "START") {
                int end_time;
                if (i + 1 < events_.size() && events_[i + 1].pid == event.pid) {
                    end_time = events_[i + 1].time;
                } else {
                    end_time = event.time + 1;
                }

                for (int t = event.time; t < end_time; t++) {
                    line[t] = '=';
                }
            }
        }

        // Output process lines
        for (const auto& [pid, line] : process_lines) {
            ss << "P" << std::setw(3) << pid << ": ";
            for (char c : line) {
                ss << std::setw(3) << (c == ' ' ? '.' : c);
            }
            ss << "\n";
        }

        return ss.str();
    }

    std::string generate_metrics(const SimulationStats& stats) const {
        std::stringstream ss;
        ss << "\nPerformance Metrics:\n";
        ss << "==================\n\n";
        ss << std::fixed << std::setprecision(2);
        ss << "Average Waiting Time:     " << stats.avg_waiting_time << " units\n";
        ss << "Average Turnaround Time:  " << stats.avg_turnaround_time << " units\n";
        ss << "Average Response Time:    " << stats.avg_response_time << " units\n";
        ss << "Total Context Switches:   " << stats.total_context_switches << "\n";
        ss << "Completed Processes:      " << stats.completed_processes << "\n";
        ss << "CPU Utilization:         " << calculate_cpu_utilization() * 100 << "%\n";
        return ss.str();
    }

private:
    double calculate_cpu_utilization() const {
        if (events_.empty()) return 0.0;
        
        int total_time = events_.back().time + 1;
        int busy_time = 0;
        
        for (size_t i = 0; i < events_.size(); i++) {
            if (events_[i].event_type == "START") {
                int end_time;
                if (i + 1 < events_.size() && events_[i + 1].pid == events_[i].pid) {
                    end_time = events_[i + 1].time;
                } else {
                    end_time = events_[i].time + 1;
                }
                busy_time += end_time - events_[i].time;
            }
        }
        
        return static_cast<double>(busy_time) / total_time;
    }

    std::vector<TimelineEvent> events_;
};

} // namespace cpu_scheduler 