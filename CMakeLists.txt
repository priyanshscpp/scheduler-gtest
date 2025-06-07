cmake_minimum_required(VERSION 3.14)
project(cpu-scheduler-simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch nlohmann_json
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
)
FetchContent_MakeAvailable(json)

# Fetch CLI11
FetchContent_Declare(cli11
    URL https://github.com/CLIUtils/CLI11/archive/v2.3.2.tar.gz
)
FetchContent_MakeAvailable(cli11)

# Fetch GoogleTest
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.tar.gz
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Main executable
add_executable(cpu-scheduler
    src/main.cpp
    src/core/simulator.cpp
    src/core/scheduler.cpp
)

target_link_libraries(cpu-scheduler
    PRIVATE
    nlohmann_json::nlohmann_json
    CLI11::CLI11
)

# Tests executable
add_executable(scheduler-tests
    tests/scheduler_test.cpp
    src/core/simulator.cpp
    src/core/scheduler.cpp
)

target_link_libraries(scheduler-tests
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    nlohmann_json::nlohmann_json
)

# Install targets
install(TARGETS cpu-scheduler DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/benchmarks DESTINATION share/cpu-scheduler)

# Enable testing
enable_testing()
add_test(NAME scheduler_tests COMMAND scheduler-tests) 