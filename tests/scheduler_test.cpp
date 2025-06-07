#include <gtest/gtest.h>
#include "core/simulator.hpp"
#include "algorithms/round_robin.hpp"
#include "algorithms/fcfs.hpp"
#include "algorithms/sjf.hpp"
#include "algorithms/priority.hpp"

using namespace cpu_scheduler;

class SchedulerTest : public ::testing::Test {
protected:
    void SetUp() override {
        procs = {
            {0, 5, 1},
            {2, 3, 2},
            {4, 4, 1},
            {6, 2, 3}
        };
    }

    void run_sim(Simulator& sim) {
        for (const auto& [at, bt, prio] : procs) {
            sim.add_process(at, bt, prio);
        }
        stats = sim.run();
    }

    std::vector<std::tuple<int, int, int>> procs;
    SimulationStats stats;
};

TEST_F(SchedulerTest, RR) {
    auto s = std::make_unique<RoundRobinScheduler>(2);
    Simulator sim(std::move(s));
    run_sim(sim);

    EXPECT_GT(stats.avg_waiting_time, 0);
    EXPECT_GT(stats.avg_turnaround_time, 0);
    EXPECT_EQ(stats.completed_processes, procs.size());
    EXPECT_GT(stats.total_context_switches, 0);
}

TEST_F(SchedulerTest, FCFS) {
    auto s = std::make_unique<FCFSScheduler>();
    Simulator sim(std::move(s));
    run_sim(sim);

    EXPECT_GT(stats.avg_waiting_time, 0);
    EXPECT_GT(stats.avg_turnaround_time, 0);
    EXPECT_EQ(stats.completed_processes, procs.size());
}

TEST_F(SchedulerTest, SJF) {
    auto s = std::make_unique<SJFScheduler>();
    Simulator sim(std::move(s));
    run_sim(sim);

    EXPECT_GT(stats.avg_waiting_time, 0);
    EXPECT_GT(stats.avg_turnaround_time, 0);
    EXPECT_EQ(stats.completed_processes, procs.size());
}

TEST_F(SchedulerTest, Priority) {
    auto s = std::make_unique<PriorityScheduler>(true);
    Simulator sim(std::move(s));
    run_sim(sim);

    EXPECT_GT(stats.avg_waiting_time, 0);
    EXPECT_GT(stats.avg_turnaround_time, 0);
    EXPECT_EQ(stats.completed_processes, procs.size());
}

TEST_F(SchedulerTest, ContextSwitch) {
    auto s = std::make_unique<RoundRobinScheduler>(2);
    Simulator sim(std::move(s), 1);
    run_sim(sim);

    EXPECT_GT(stats.total_context_switches, 0);
    EXPECT_GT(stats.avg_waiting_time, 0);
}

TEST_F(SchedulerTest, Empty) {
    auto s = std::make_unique<FCFSScheduler>();
    Simulator sim(std::move(s));
    stats = sim.run();

    EXPECT_EQ(stats.completed_processes, 0);
    EXPECT_EQ(stats.total_context_switches, 0);
    EXPECT_EQ(stats.avg_waiting_time, 0);
}

TEST_F(SchedulerTest, SingleProc) {
    auto s = std::make_unique<FCFSScheduler>();
    Simulator sim(std::move(s));
    sim.add_process(0, 5, 1);
    stats = sim.run();

    EXPECT_EQ(stats.completed_processes, 1);
    EXPECT_EQ(stats.avg_waiting_time, 0);
    EXPECT_EQ(stats.avg_turnaround_time, 5);
}

TEST_F(SchedulerTest, SameArrival) {
    auto s = std::make_unique<SJFScheduler>();
    Simulator sim(std::move(s));
    
    sim.add_process(0, 5, 1);
    sim.add_process(0, 2, 1);
    sim.add_process(0, 3, 1);
    
    stats = sim.run();
    EXPECT_EQ(stats.completed_processes, 3);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 