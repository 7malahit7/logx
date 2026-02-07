#pragma once

#include <string_view>
#include "logx/core/parse_result.hpp"

namespace logx {

    class ILogParser {
    public:
            virtual ParseResult parse(std::string_view) const = 0;
            virtual ~ILogParser() = default;
        };

}