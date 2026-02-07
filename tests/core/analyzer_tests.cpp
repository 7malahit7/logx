#include <logx/core/analyzer.hpp>
#include <gtest/gtest.h>

namespace logx {

    static LogEntry new_entry(LogLevel level) {
        return {
        .timestamp  = "2026-01-01 00:00:00",
        .level      = level,
        .module     = "test",
        .message    = "info"
        };
    }

    TEST(AnalyzerTest, InitialState) {
        Analyzer analyzer;
        const auto& result = analyzer.result();

        EXPECT_EQ(result.total, 0);
        EXPECT_EQ(result.parse_errors, 0);
    }

    TEST(AnalyzerTest, AddSingleEntry_IncrementsTotalAndLevel) {


        Analyzer analyzer;
        LogEntry entry = new_entry(LogLevel::INFO);

        analyzer.add_entry(entry);

        const auto& result = analyzer.result();

        EXPECT_EQ(result.total, 1);
        EXPECT_EQ(result.parse_errors, 0);

        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::INFO) ], 1);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::DEBUG)], 0);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::ERROR)], 0);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::WARN) ], 0);
    }

    TEST(AnalyzerTest, AddMultipleEntries_AggregatesCorrectly) {
        Analyzer analyzer;

        analyzer.add_entry(new_entry(LogLevel::INFO));
        analyzer.add_entry(new_entry(LogLevel::DEBUG));
        analyzer.add_entry(new_entry(LogLevel::ERROR));
        analyzer.add_entry(new_entry(LogLevel::WARN));
        analyzer.add_entry(new_entry(LogLevel::INFO));

        const auto& result = analyzer.result();

        EXPECT_EQ(result.total, 5);
        EXPECT_EQ(result.parse_errors, 0);

        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::INFO) ], 2);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::DEBUG)], 1);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::ERROR)], 1);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::WARN) ], 1);
    }

    TEST(AnalyzerTest, OnParseError_IncrementsErrorCounterOnly) {
        Analyzer analyzer;
        analyzer.on_parse_error();
        const auto& result = analyzer.result();

        EXPECT_EQ(result.parse_errors, 1);
        EXPECT_EQ(result.total, 0);

        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::INFO) ], 0);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::DEBUG)], 0);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::ERROR)], 0);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::WARN) ], 0);
    }

}

