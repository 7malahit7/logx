#include "logx/report/text_report_writer.hpp"

#include  <gtest/gtest.h>
#include  <logx/core/analysis_result.hpp>


logx::AnalysisResult make_analysis_result(
    size_t parse_errors_,
    size_t total_,
    size_t info_count_,
    size_t debug_count_,
    size_t warning_count_,
    size_t error_count_) {

    logx::AnalysisResult result{};

    result.parse_errors = parse_errors_;
    result.total = total_;
    result.log_level_counters[logx::log_level_to_index(logx::LogLevel::INFO)] = info_count_;
    result.log_level_counters[logx::log_level_to_index(logx::LogLevel::DEBUG)] = debug_count_;
    result.log_level_counters[logx::log_level_to_index(logx::LogLevel::WARN)] = warning_count_;
    result.log_level_counters[logx::log_level_to_index(logx::LogLevel::ERROR)] = error_count_;

    return result;
}

TEST(TextReportWriter, TextCorrectReportWriter) {
    logx::AnalysisResult result = make_analysis_result(0,20,5,5,5,5);

    std::string expected_str =
        "Total entries: 20\n"
        "INFO: 5\n"
        "DEBUG: 5\n"
        "WARN: 5\n"
        "ERROR: 5\n";

    logx::TextReportWriter writer;

    ASSERT_EQ(writer.write(result), expected_str);
}

TEST(TextReportWriter, TextZeroReportWriter) {
    logx::AnalysisResult result = make_analysis_result(0,0,0,0,0,0);

    std::string expected_str =
        "Total entries: 0\n"
        "INFO: 0\n"
        "DEBUG: 0\n"
        "WARN: 0\n"
        "ERROR: 0\n";

    logx::TextReportWriter writer;

    ASSERT_EQ(writer.write(result), expected_str);
}

TEST(TextReportWriter, LinesInCorrectOrder) {
    logx::AnalysisResult result = make_analysis_result(0,10,3,1,5,1);

    logx::TextReportWriter writer;
    std::string result_str = writer.write(result);

    size_t pos_total = result_str.find("Total");
    size_t pos_info = result_str.find("INFO");
    size_t pos_debug = result_str.find("DEBUG");
    size_t pos_warn = result_str.find("WARN");
    size_t pos_error = result_str.find("ERROR");

    ASSERT_NE(pos_total, std::string::npos);
    ASSERT_NE(pos_info, std::string::npos);
    ASSERT_NE(pos_debug, std::string::npos);
    ASSERT_NE(pos_warn, std::string::npos);
    ASSERT_NE(pos_error, std::string::npos);


    ASSERT_LT(pos_total, pos_info);
    ASSERT_LT(pos_info, pos_debug);
    ASSERT_LT(pos_debug, pos_warn);
    ASSERT_LT(pos_warn, pos_error);
}