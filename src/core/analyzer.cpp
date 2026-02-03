#include <logx/core/analyzer.hpp>

namespace logx {
    Analyzer::Analyzer() {
        _analysis_result.total                   = 0;
        _analysis_result.parse_errors            = 0;
        _analysis_result.log_level_counters.fill(0);
    }

    void Analyzer::add_entry(const LogEntry& entry) {
        ++_analysis_result.total;
        ++_analysis_result.log_level_counters[static_cast<size_t>(entry.level)];
    }

    void Analyzer::on_parse_error() {
        ++_analysis_result.parse_errors;
    }

    const AnalysisResult& Analyzer::result() const {
        return _analysis_result;
    }
}