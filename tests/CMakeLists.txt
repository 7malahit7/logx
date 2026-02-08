
add_executable( logx_tests
    core/analyzer_tests.cpp
    parser/simple_text_log_parser_test.cpp
    parser/json_report_writer_test.cpp
    parser/text_report_writer_test.cpp
)

target_link_libraries(logx_tests
        PRIVATE
            gtest_main
            logx_core
)