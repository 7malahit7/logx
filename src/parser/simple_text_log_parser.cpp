#include<logx/parser/simple_text_log_parser.hpp>

namespace logx {
    static int two_digits_to_int(std::string_view two_digits) {
        return (two_digits[0] - '0') * 10 + (two_digits[1] - '0');
    }
    static bool is_all_digits(std::string_view substr) {
        for (auto c : substr)
            if (!std::isdigit(c)) return false;
        return true;
    }



    bool SimpleTextLogParser::isValidTimeStamp(std::string_view time_stamp) {
        if (time_stamp.size() != 19) return false;

        if (time_stamp[4]  != '-' ||
            time_stamp[7]  != '-' ||
            time_stamp[10] != ' ' ||
            time_stamp[13] != ':' ||
            time_stamp[16] != ':'
        ) return false;

        //YYYY
        if (!is_all_digits(time_stamp.substr(0,4)))return false;
        //MM
        if (!is_all_digits(time_stamp.substr(5,2)))return false;
        //DD
        if (!is_all_digits(time_stamp.substr(8,2)))return false;
        //HH
        if (!is_all_digits(time_stamp.substr(11,2)))return false;
        //MM
        if (!is_all_digits(time_stamp.substr(14,2)))return false;
        //SS
        if (!is_all_digits(time_stamp.substr(17,2)))return false;

        const int month  = two_digits_to_int(time_stamp.substr(5,2));
        const int day    = two_digits_to_int(time_stamp.substr(8,2));
        const int hour   = two_digits_to_int(time_stamp.substr(11,2));
        const int minute = two_digits_to_int(time_stamp.substr(14,2));
        const int second = two_digits_to_int(time_stamp.substr(17,2));

        if (
            (month  < 1 || month    > 12) ||
            (day    < 1 || day      > 31) ||
            (hour   < 0 || hour     > 23) ||
            (minute < 0 || minute   > 59) ||
            (second < 0 || second   > 59)
        ) return false;
        return true;
    }

    std::optional<LogLevel> SimpleTextLogParser::isValidLogLevel(std::string_view log_level) {
        if (log_level == "ERROR") return LogLevel::ERROR;
        if (log_level == "INFO") return LogLevel::INFO;
        if (log_level == "DEBUG") return LogLevel::DEBUG;
        if (log_level == "WARN") return LogLevel::WARN;
        return std::nullopt;
    }

    bool SimpleTextLogParser::isValidLogModule(std::string_view log_module) {
        return !log_module.empty();
    }

    bool SimpleTextLogParser::isValidLogMessage(std::string_view log_message) {
        for (size_t i {}; i < log_message.size();) {
            unsigned char current_char = static_cast<unsigned char>(log_message[i]);

            //NBSP
            if (current_char == 0xC2) {
                if ( i+1 < log_message.size() ){
                    const unsigned char next_char = static_cast<unsigned char>(log_message[i + 1]);
                    if (next_char == 0xA0){
                        i+=2;
                        continue;
                    }
                    else return true;
                }
                else return true;
            }

            //IDEOGRAPHIC SPACE
            else if (current_char == 0xE3) {
                if (i+2 < log_message.size()) {
                    const unsigned char next_char = static_cast<unsigned char>(log_message[i + 1]);
                    const unsigned char next_plus_one_char = static_cast<unsigned char>(log_message[i + 2]);
                    if ( next_char == 0x80 && next_plus_one_char == 0x80) {
                        i+=3;
                        continue;
                    }
                    else return true;
                }
                else return true;
            }
            //ASCII
            else if (!isspace(current_char)) return true;

            ++i;
        }
        return false;
    }

    ParseResult SimpleTextLogParser::parse(std::string_view text) const {
        return  ParseResult::failure(ParseError::InvalidFormat);
    }
}
