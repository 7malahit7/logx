#pragma once
#include <logx/core/analysis_result.hpp>

namespace logx {
    class TextReportWriter {
        public:
            std::string write(const AnalysisResult& entry_result) const;
    };
}