#include<logx/parser/simple_text_log_parser.hpp>


logx::ParseResult logx::SimpleTextLogParser::parse(std::string_view text) const {
    return  ParseResult::failure(ParseError::InvalidFormat);
}
