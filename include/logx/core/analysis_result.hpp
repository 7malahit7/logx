#pragma once
#include <array>
#include <cstddef>

#include "log_level.hpp"

namespace logx {
    struct AnalysisResult {
        std::size_t total;
        std::size_t parse_errors;

        std::array<std::size_t, static_cast<size_t>(LogLevel::LEVEL_COUNT)> log_level_counters;
        AnalysisResult() = default;
    };
}
