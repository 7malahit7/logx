#pragma once

#include <string_view>

namespace logx {
    enum class LogLevel;

    constexpr std::size_t log_level_to_index (LogLevel level) {
        return static_cast<std::size_t>(level) ;
    }
    enum class LogLevel {
        INFO,
        DEBUG,
        WARN,
        ERROR,
        LEVEL_COUNT
    };

    constexpr std::string_view to_string(LogLevel level) {
        switch (level) {
            case LogLevel::INFO: return "INFO";
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::WARN: return "WARN";
            case LogLevel::ERROR: return "ERROR";
            default: return "UNKNOWN";
        }
    }
}