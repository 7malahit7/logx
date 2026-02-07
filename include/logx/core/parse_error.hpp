#pragma once

namespace logx {
    enum class ParseError {
        InvalidTimestamp,
        InvalidLevel,
        MissingField,
        InvalidFormat,
        EmptyMessage
    };
}