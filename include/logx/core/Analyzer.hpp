#pragma once

#include "analysis_result.hpp"
#include "log_entry.hpp"


namespace logx {
    class Analyzer {
    private:
        AnalysisResult _analysis_result;
    public:
        Analyzer();

        void add_entry(const LogEntry& entry);
        void on_parse_error();

        const AnalysisResult& result() const;
    };
}
