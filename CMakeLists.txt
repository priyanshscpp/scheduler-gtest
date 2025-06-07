cmake_minimum_required(VERSION 3.14)
project(cpu-scheduler-simulator-cpp 
        VERSION 1.0.0
        DESCRIPTION "A production-grade CPU Scheduler Simulator"
        LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable testing
include(CTest)
enable_testing()

# Find required packages
find_package(GTest REQUIRED)

# Set compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -pedantic)
endif()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmarks)

# Install targets
install(TARGETS cpu-scheduler-simulator
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib) 