#pragma once

#include <string_view>

namespace logx {
    enum class LogLevel {
        Info,
        Debug,
        Warn,
        Error
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