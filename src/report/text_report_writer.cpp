#include <string>
#include <logx/report/text_report_writer.hpp>
namespace logx {
    std::string TextReportWriter::write(const AnalysisResult &entry_result) const{
        std::string result;
        result.reserve(64);

        result+= "Total entries: ";
        result+= std::to_string(entry_result.total);
        result+= '\n';

        result+= "INFO: ";
        result+= std::to_string(entry_result.log_level_counters[log_level_to_index(LogLevel::INFO)]);
        result+= '\n';

        result+= "DEBUG: ";
        result+= std::to_string(entry_result.log_level_counters[log_level_to_index(LogLevel::DEBUG)]);
        result+= '\n';

        result+= "WARN: ";
        result+= std::to_string(entry_result.log_level_counters[log_level_to_index(LogLevel::WARN)]);
        result+= '\n';

        result+= "ERROR: ";
        result+= std::to_string(entry_result.log_level_counters[log_level_to_index(LogLevel::ERROR)]);
        result+= '\n';

        return result;
    }
}