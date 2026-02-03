#include "log_level.hpp"
#include <string>


namespace logx {
    struct LogEntry {
        std::string timestamp;
        LogLevel level;
        std::string module;
        std::string message;

        bool operator==(const LogEntry&) const = default;
    };

}