#pragma once
#include "ilog_parser.hpp"

namespace  logx {

    class SimpleTextLogParser : public ILogParser{
        public:
            SimpleTextLogParser() {};
            ParseResult parse(std::string_view log_line) const override;
    };
}
