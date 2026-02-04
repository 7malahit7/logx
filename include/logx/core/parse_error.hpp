#pragma once

namespace logx {
    enum class ParseError {
        InvalidTimestamp,
        InvalidLevel,
        InvalidModule,
        MissingField,
        InvalidFormat,
        EmptyMessage
    };
}