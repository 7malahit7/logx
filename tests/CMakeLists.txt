
add_executable( logx_tests
    core/analyzer_tests.cpp
)

target_link_libraries(logx_tests
        PRIVATE
            gtest_main
            logx_core
)