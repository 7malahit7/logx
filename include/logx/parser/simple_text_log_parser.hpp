#pragma once
#include <optional>

#include "ilog_parser.hpp"

namespace  logx {

    class SimpleTextLogParser : public ILogParser{
        private:
            static bool isValidTimeStamp(std::string_view time_stamp);
            static std::optional<LogLevel> isValidLogLevel(std::string_view log_level);
            static bool isValidLogModule(std::string_view log_module);
            static bool isValidLogMessage(std::string_view log_message);

        public:
            SimpleTextLogParser() = default;
            ParseResult parse(std::string_view log_line) const override;
    };
}
