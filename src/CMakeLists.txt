add_executable(cpu-scheduler
    main.cpp
)

target_include_directories(cpu-scheduler
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(cpu-scheduler 
    PRIVATE 
        cxx_std_17
)

# Add compiler warnings
if(MSVC)
    target_compile_options(cpu-scheduler PRIVATE /W4 /WX)
else()
    target_compile_options(cpu-scheduler PRIVATE -Wall -Wextra -Werror -pedantic)
endif()

# Install the target
install(TARGETS cpu-scheduler
    RUNTIME DESTINATION bin
) 