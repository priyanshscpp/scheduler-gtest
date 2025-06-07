#include "core/simulator.hpp"
#include "algorithms/round_robin.hpp"
#include "algorithms/fcfs.hpp"
#include "algorithms/sjf.hpp"
#include "algorithms/priority.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <CLI/CLI.hpp>

using json = nlohmann::json;
using namespace cpu_scheduler;

struct Config {
    std::string algo = "rr";
    int quantum = 4;
    int ctx_switch = 0;
    std::string workload;
    bool verbose = false;
    bool preempt = true;
};

std::vector<std::tuple<int, int, int>> load_workload(const std::string& filename) {
    std::vector<std::tuple<int, int, int>> workload;
    std::ifstream file(filename);
    json j;
    file >> j;

    for (const auto& p : j["processes"]) {
        workload.emplace_back(
            p["arrival_time"].get<int>(),
            p["burst_time"].get<int>(),
            p["priority"].get<int>()
        );
    }
    return workload;
}

void print_help() {
    std::cout << "Available Scheduling Algorithms:\n"
              << "  rr    - Round Robin\n"
              << "  fcfs  - First Come First Serve\n"
              << "  sjf   - Shortest Job First\n"
              << "  prio  - Priority Scheduling\n\n"
              << "Example Usage:\n"
              << "  ./cpu-scheduler -a rr -q 4\n"
              << "  ./cpu-scheduler -a prio --preemptive\n"
              << "  ./cpu-scheduler -a sjf -w workload.json\n";
}

int main(int argc, char** argv) {
    Config cfg;
    CLI::App app{"CPU Scheduler"};

    app.add_option("-a,--algo", cfg.algo, "algorithm (rr/fcfs/sjf/prio)")
        ->default_str("rr");
    app.add_option("-q", cfg.quantum, "quantum for RR")
        ->default_val(4);
    app.add_option("-c", cfg.ctx_switch, "context switch overhead")
        ->default_val(0);
    app.add_option("-w", cfg.workload, "workload file");
    app.add_flag("-v", cfg.verbose, "verbose output");
    app.add_flag("-p", cfg.preempt, "preemptive scheduling");
    app.add_flag("-h,--help", [](){ print_help(); exit(0); }, 
                 "Show detailed help");

    CLI11_PARSE(app, argc, argv);

    std::unique_ptr<Scheduler> scheduler;
    if (cfg.algo == "rr") {
        scheduler = std::make_unique<RoundRobinScheduler>(cfg.quantum);
    } else if (cfg.algo == "fcfs") {
        scheduler = std::make_unique<FCFSScheduler>();
    } else if (cfg.algo == "sjf") {
        scheduler = std::make_unique<SJFScheduler>();
    } else if (cfg.algo == "prio") {
        scheduler = std::make_unique<PriorityScheduler>(cfg.preempt);
    } else {
        std::cerr << "Unknown algorithm: " << cfg.algo << std::endl;
        print_help();
        return 1;
    }

    Simulator sim(std::move(scheduler), cfg.ctx_switch);

    std::vector<std::tuple<int, int, int>> workload;
    if (!cfg.workload.empty()) {
        try {
            workload = load_workload(cfg.workload);
        } catch (const std::exception& e) {
            std::cerr << "Failed to load workload: " << e.what() << std::endl;
            return 1;
        }
    } else {
        workload = {
            {0, 5, 1},
            {2, 3, 2},
            {4, 4, 1},
            {6, 2, 3}
        };
    }

    for (const auto& [arrival, burst, prio] : workload) {
        sim.add_process(arrival, burst, prio);
    }

    auto stats = sim.run();

    std::cout << "\nSimulation Results:\n"
              << "==================\n"
              << stats.to_string() << std::endl;

    return 0;
} 