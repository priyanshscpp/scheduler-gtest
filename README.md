# CPU Scheduler Simulator


[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

A production-grade CPU scheduler simulator that implements various scheduling algorithms with detailed performance analysis and visualization capabilities.

## 🎯 Features

- Multiple scheduling algorithms:
  - First Come First Serve (FCFS)
  - Shortest Job First (SJF)
  - Round Robin (RR)
  - Priority Scheduling
  - Multilevel Feedback Queue
- Real-time visualization of process scheduling
- Detailed performance metrics and benchmarking
- Support for custom workload scenarios via JSON/CSV
- Memory management simulation
- Context switch overhead simulation
- Comprehensive test suite


## Component Architecture Diagram 

image.png


## 🚀 Getting Started

### Prerequisites

- CMake 3.14 or higher
- C++17 compatible compiler
- Google Test (for unit testing)
- Docker (optional)

### Building from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/cpu-scheduler-simulator-cpp.git
cd cpu-scheduler-simulator-cpp

# Create build directory
mkdir build && cd build

# Configure and build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build .

# Run tests
ctest --output-on-failure
```

### Using Docker

```bash
# Build the Docker image
docker build -t cpu-scheduler .

# Run the simulator
docker run -it cpu-scheduler
```

## 📊 Usage

### Basic Usage

```bash
# Run with default FCFS scheduler
./cpu-scheduler --algorithm fcfs

# Run with Round Robin (quantum = 4)
./cpu-scheduler --algorithm rr --quantum 4

# Load custom workload
./cpu-scheduler --workload workloads/example.json
```

### Sample Output

```
Time Quantum: 4
Process ID | Arrival Time | Burst Time | Priority
------------------------------------------------
1          | 0           | 10         | 2
2          | 2           | 5          | 1
3          | 4           | 8          | 3

Scheduling Timeline:
[P1][P2][P1][P3][P1]...
```

## 🔧 Architecture

The simulator is built with a modular, object-oriented design:

```
src/
├── core/
│   ├── process.hpp
│   ├── scheduler.hpp
│   └── simulator.hpp
├── algorithms/
│   ├── fcfs.hpp
│   ├── sjf.hpp
│   └── round_robin.hpp
└── utils/
    ├── metrics.hpp
    └── visualization.hpp
```

## 📈 Benchmarks

See [BENCHMARKS.md](BENCHMARKS.md) for detailed performance comparisons between different scheduling algorithms.

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines.

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Operating Systems Concepts (Silberschatz et al.)
- Modern Operating Systems (Tanenbaum)
# scheduler-gtest
