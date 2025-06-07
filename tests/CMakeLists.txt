enable_testing()

add_executable(scheduler_test
    scheduler_test.cpp
)

target_include_directories(scheduler_test
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(scheduler_test
    PRIVATE
        GTest::GTest
        GTest::Main
)

add_test(NAME scheduler_test COMMAND scheduler_test) 