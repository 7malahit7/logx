#include <logx/core/parse_result.hpp>
#include <cassert>
logx::ParseResult::ParseResult(std::variant<LogEntry, ParseError> value) : _value(std::move(value)){}

logx::ParseResult logx::ParseResult::success(LogEntry log_entry) {
    return ParseResult(std::move(log_entry));
}

logx::ParseResult logx::ParseResult::failure(ParseError error_type) {
    return ParseResult(std::move(error_type));
}

bool logx::ParseResult::ok() const {
    return std::holds_alternative<LogEntry>(_value);
}

const logx::ParseError & logx::ParseResult::error() const {
    assert(!ok());
    return std::get<ParseError>(_value);
}

const logx::LogEntry & logx::ParseResult::value() const {
    assert(ok());
    return std::get<LogEntry>(_value);
}
