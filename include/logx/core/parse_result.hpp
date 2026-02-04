#pragma once

#include <variant>

#include "log_entry.hpp"
#include "parse_error.hpp"

namespace  logx {

class ParseResult {
    private:
        std::variant<LogEntry, ParseError> _value;
        ParseResult(std::variant<LogEntry, ParseError> value); //Задаёт вэлью в зависимости от варианта
    public:
        static ParseResult success(LogEntry log_entry);
        static ParseResult failure(ParseError error_type);

        bool ok() const;

        const ParseError& error() const;  // ok() == false
        const LogEntry&  value() const;  // ok() == true
    };
}