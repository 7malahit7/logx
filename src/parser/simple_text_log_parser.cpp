#include<logx/parser/simple_text_log_parser.hpp>

 namespace LogIndexHelper{
    static constexpr size_t TimeStampYearStartIndex      = 0;
    static constexpr size_t TimeStampMonthStartIndex    = 5;
    static constexpr size_t TimeStampDayStartIndex      = 8;
    static constexpr size_t TimeStampHourStartIndex     = 11;
    static constexpr size_t TimeStampMinStartIndex      = 14;
    static constexpr size_t TimeStampSecondStartIndex   = 17;
    static constexpr size_t TimeStampLength             = 19;
};
namespace lih = LogIndexHelper;

namespace logx {


    static int  two_digits_to_int(std::string_view two_digits) {
        return (two_digits[0] - '0') * 10 + (two_digits[1] - '0');
    }
    static bool is_all_digits(std::string_view substr) {
        for (auto c : substr)
            if (!std::isdigit(static_cast<unsigned char>(c))) return false;
        return true;
    }
    static bool can_read_next_n(size_t cursor_index, size_t length, size_t need) {
        return cursor_index + need <= length;
    }

    bool SimpleTextLogParser::isValidTimeStamp(std::string_view time_stamp) {
        if (time_stamp.size() != LogIndexHelper::TimeStampLength) return false;

        if (time_stamp[lih::TimeStampMonthStartIndex - 1]    != '-' ||
            time_stamp[lih::TimeStampDayStartIndex - 1]      != '-' ||
            time_stamp[lih::TimeStampHourStartIndex - 1]     != ' ' ||
            time_stamp[lih::TimeStampMinStartIndex - 1]      != ':' ||
            time_stamp[lih::TimeStampSecondStartIndex - 1]   != ':'
        ) return false;

        //YYYY
        if (!is_all_digits(time_stamp.substr(lih::TimeStampYearStartIndex,4)))   return false;
        //MM
        if (!is_all_digits(time_stamp.substr(lih::TimeStampMonthStartIndex,2)))  return false;
        //DD
        if (!is_all_digits(time_stamp.substr(lih::TimeStampDayStartIndex,2)))    return false;
        //HH
        if (!is_all_digits(time_stamp.substr(lih::TimeStampHourStartIndex,2)))   return false;
        //MM
        if (!is_all_digits(time_stamp.substr(lih::TimeStampMinStartIndex,2)))    return false;
        //SS
        if (!is_all_digits(time_stamp.substr(lih::TimeStampSecondStartIndex,2))) return false;

        const int month  = two_digits_to_int(time_stamp.substr(lih::TimeStampMonthStartIndex,2));
        const int day    = two_digits_to_int(time_stamp.substr(lih::TimeStampDayStartIndex,2));
        const int hour   = two_digits_to_int(time_stamp.substr(lih::TimeStampHourStartIndex,2));
        const int minute = two_digits_to_int(time_stamp.substr(lih::TimeStampMinStartIndex,2));
        const int second = two_digits_to_int(time_stamp.substr(lih::TimeStampSecondStartIndex,2));

        if (
            (month  < 1 || month    > 12) ||
            (day    < 1 || day      > 31) ||
            (hour   < 0 || hour     > 23) ||
            (minute < 0 || minute   > 59) ||
            (second < 0 || second   > 59)
        ) return false;
        return true;
    }

    std::optional<LogLevel> SimpleTextLogParser::logLevelValidation(std::string_view log_level) {
        if (log_level == "ERROR") return LogLevel::ERROR;
        if (log_level == "INFO") return LogLevel::INFO;
        if (log_level == "DEBUG") return LogLevel::DEBUG;
        if (log_level == "WARN") return LogLevel::WARN;
        return std::nullopt;
    }

    bool SimpleTextLogParser::isValidLogMessage(std::string_view log_message) {
        for (size_t i {}; i < log_message.size();) {
            const auto current_char = static_cast<unsigned char>(log_message[i]);

            //NBSP
            if (current_char == 0xC2) {
                if ( i+1 < log_message.size() ){
                    const auto next_char = static_cast<unsigned char>(log_message[i + 1]);
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
                    const auto next_char = static_cast<unsigned char>(log_message[i + 1]);
                    const auto next_plus_one_char = static_cast<unsigned char>(log_message[i + 2]);
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

    ParseResult SimpleTextLogParser::parse(std::string_view log_entry) const {
        size_t cursor = 0;
        size_t log_length = log_entry.size();
        size_t module_start_index;
        LogEntry entry;

        //Timestamp
        if (!can_read_next_n(cursor, log_length, lih::TimeStampLength)) return ParseResult::failure(ParseError::InvalidFormat);
        if (!isValidTimeStamp(log_entry.substr(cursor, lih::TimeStampLength)))
            return ParseResult::failure(ParseError::InvalidTimestamp);
        cursor+=lih::TimeStampLength;

        if (!can_read_next_n(cursor, log_length,1)) return ParseResult::failure(ParseError::InvalidFormat);
        if (log_entry[cursor] != ' ') return ParseResult::failure(ParseError::InvalidFormat);
        ++cursor;

        entry.timestamp = log_entry.substr(0, lih::TimeStampLength);

        //Level
        if (!can_read_next_n(cursor, log_length,1)) return ParseResult::failure(ParseError::InvalidFormat);
        if (log_entry[cursor] != '[') return ParseResult::failure(ParseError::InvalidFormat);
        ++cursor;

        size_t level_end_bracket_index = log_entry.find(']', cursor);
        if (level_end_bracket_index == std::string::npos)return ParseResult::failure(ParseError::InvalidFormat);
        auto level_opt = logLevelValidation(log_entry.substr(cursor, level_end_bracket_index - cursor));
        if (!level_opt) return ParseResult::failure(ParseError::InvalidLevel);

        cursor = level_end_bracket_index + 1;
        if (!can_read_next_n(cursor, log_length,1)) return ParseResult::failure(ParseError::InvalidFormat);
        if (log_entry[cursor] != ' ') return ParseResult::failure(ParseError::InvalidFormat);
        ++cursor;

        entry.level = *level_opt;

        //Module
        if (!can_read_next_n(cursor, log_length,1)) return ParseResult::failure(ParseError::InvalidFormat);
        if (log_entry[cursor] != '[') return ParseResult::failure(ParseError::InvalidFormat);
        ++cursor;

        module_start_index = cursor;

        if (!can_read_next_n(cursor, log_length,1)) return ParseResult::failure(ParseError::InvalidFormat);
        if (log_entry[cursor] == ']') return ParseResult::failure(ParseError::InvalidFormat);


        cursor = log_entry.find(']', cursor);
        if (cursor == std::string::npos) return ParseResult::failure(ParseError::InvalidFormat);
        entry.module = log_entry.substr(module_start_index, cursor - module_start_index);
        ++cursor;

        if (!can_read_next_n(cursor, log_length,1)) return ParseResult::failure(ParseError::InvalidFormat);
        if (log_entry[cursor] != ' ') return ParseResult::failure(ParseError::InvalidFormat);
        ++cursor;

        //Message
        if (!isValidLogMessage(log_entry.substr(cursor))) {
            return ParseResult::failure(ParseError::EmptyMessage);
        }
        entry.message = log_entry.substr(cursor);
        return ParseResult::success(entry);
    }
}
