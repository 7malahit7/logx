#pragma once

#include <string_view>

namespace logx {
    enum class LogLevel;

    constexpr std::size_t log_level_to_index (LogLevel level) {
        return static_cast<std::size_t>(level) ;
    }
    enum class LogLevel {
        Info,
        Debug,
        Warn,
        Error,
        LEVEL_COUNT
    };

    constexpr std::string_view to_string(LogLevel level) {
        switch (level) {
            case LogLevel::Info: return "INFO";
            case LogLevel::Debug: return "DEBUG";
            case LogLevel::Warn: return "WARN";
            case LogLevel::Error: return "ERROR";
        }
        return "UNKNOWN";
    }
}