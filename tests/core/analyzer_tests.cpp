#include <logx/core/analyzer.hpp>
#include <gtest/gtest.h>

namespace logx {

    constexpr std::size_t log_level_to_index (LogLevel level) {
        return static_cast<std::size_t>(level) ;
    }
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
        LogEntry entry;

        entry = new_entry(LogLevel::Info);

        analyzer.add_entry(entry);

        const auto& result = analyzer.result();

        EXPECT_EQ(result.total, 1);
        EXPECT_EQ(result.parse_errors, 0);

        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::Info) ], 1);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::Debug)], 0);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::Error)], 0);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::Warn) ], 0);
    }

    TEST(AnalyzerTest, AddMultipleEntries_AggregatesCorrectly) {
        Analyzer analyzer;

        analyzer.add_entry(new_entry(LogLevel::Info));
        analyzer.add_entry(new_entry(LogLevel::Debug));
        analyzer.add_entry(new_entry(LogLevel::Error));
        analyzer.add_entry(new_entry(LogLevel::Warn));
        analyzer.add_entry(new_entry(LogLevel::Info));

        const auto& result = analyzer.result();

        EXPECT_EQ(result.total, 5);
        EXPECT_EQ(result.parse_errors, 0);

        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::Info) ], 2);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::Debug)], 1);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::Error)], 1);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::Warn) ], 1);
    }

    TEST(AnalyzerTest, OnParseError_IncrementsErrorCounterOnly) {
        Analyzer analyzer;
        analyzer.on_parse_error();
        const auto& result = analyzer.result();

        EXPECT_EQ(result.parse_errors, 1);
        EXPECT_EQ(result.total, 0);

        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::Info) ], 0);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::Debug)], 0);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::Error)], 0);
        EXPECT_EQ(result.log_level_counters[log_level_to_index(LogLevel::Warn) ], 0);
    }

}

