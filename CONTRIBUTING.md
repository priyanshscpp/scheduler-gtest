# Contributing to CPU Scheduler Simulator

Thank you for your interest in contributing to the CPU Scheduler Simulator! This document provides guidelines and instructions for contributing to the project.

## Code of Conduct

This project adheres to a Code of Conduct that all contributors are expected to follow. Please read [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) before contributing.

## Getting Started

1. Fork the repository
2. Clone your fork:
   ```bash
   git clone https://github.com/yourusername/cpu-scheduler-simulator-cpp.git
   cd cpu-scheduler-simulator-cpp
   ```
3. Create a new branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```

## Development Environment

1. Install prerequisites:
   - CMake 3.14 or higher
   - C++17 compatible compiler
   - Google Test
   - nlohmann-json
   - CLI11

2. Build the project:
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

3. Run tests:
   ```bash
   ctest --output-on-failure
   ```

## Coding Standards

- Follow the Google C++ Style Guide
- Use modern C++ features (C++17)
- Keep functions small and focused
- Write comprehensive unit tests
- Document your code using Doxygen-style comments
- Use meaningful variable and function names

The project uses clang-format for code formatting. Before submitting a PR:

```bash
find . -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i
```

## Adding a New Scheduling Algorithm

1. Create a new header file in `include/algorithms/`
2. Inherit from the `Scheduler` base class
3. Implement all pure virtual functions
4. Add unit tests in `tests/scheduler_test.cpp`
5. Update `src/main.cpp` to support the new algorithm
6. Document the algorithm in README.md

## Pull Request Process

1. Update documentation if needed
2. Add tests for new features
3. Ensure all tests pass
4. Update CHANGELOG.md
5. Submit a PR with a clear description of changes

## Commit Messages

Follow the conventional commits specification:

```
feat: add new scheduling algorithm
fix: correct priority calculation
docs: update algorithm documentation
test: add edge case tests
refactor: improve code organization
```

## Running Benchmarks

To run performance benchmarks:

```bash
./cpu-scheduler --benchmark
```

Compare your changes against the baseline:

```bash
./cpu-scheduler --benchmark --compare-baseline
```

## Need Help?

- Check existing issues and pull requests
- Create a new issue for discussions
- Join our community chat

## License

By contributing, you agree that your contributions will be licensed under the MIT License. 