#include <gtest/gtest.h>
#include <logx/core/log_level.hpp>
#include <logx/parser/simple_text_log_parser.hpp>
#include <string_view>
#include <logx/core/parse_error.hpp>


TEST(SimpleTextLogParser, ParsesValidLine) {
    logx::SimpleTextLogParser parser;
    std::string_view line = "2026-01-12 10:15:26 [INFO] [Core] Application started";

    auto result = parser.parse(line);

    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.value().timestamp, "2026-01-12 10:15:26");
    EXPECT_EQ(result.value().level, logx::LogLevel::Info);
    EXPECT_EQ(result.value().module, "Core");
    EXPECT_EQ(result.value().message, "Application started");
}

TEST(SimpleTextLogParser, InvalidFormat)
{
    logx::SimpleTextLogParser parser;
    std::string_view line = "123";

    auto result = parser.parse(line);

    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.error(), logx::ParseError::InvalidFormat);
}
TEST(SimpleTextLogParser, InvalidLevel_UnknownLevel)
{
    logx::SimpleTextLogParser parser;
    std::string_view line = "2026-01-12 10:15:26 [INVALID123] [Core] Application started";

    auto result = parser.parse(line);

    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.error(), logx::ParseError::InvalidLevel);
}

TEST(SimpleTextLogParser, InvalidTimestamp)
{
    logx::SimpleTextLogParser parser;
    std::string_view line = "2026-99-99 99:99:99 [INFO] [Core] Application started";

    auto result = parser.parse(line);

    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.error(), logx::ParseError::InvalidTimestamp);
}
TEST(SimpleTextLogParser, EmptyMessage)
{
    logx::SimpleTextLogParser parser;
    std::string_view line = "2026-01-12 10:15:26 [INFO] [Core]";

    auto result = parser.parse(line);

    ASSERT_FALSE(result.ok());
    ASSERT_EQ(result.error(), logx::ParseError::EmptyMessage);
}