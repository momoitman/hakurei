#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "util/string_sanitizer.h"
#include "service/service_exceptions.h"

using namespace hakurei::core::util;
using namespace hakurei::core::service;
using namespace std::literals;

TEST(Hakurei_util_test, string_sanitizer)
{
    verify_string("abcdef"sv, true);
    verify_string("abcdef"sv, false);
    verify_string("abcdef\n"sv, false);
    EXPECT_THROW(verify_string("abc\0def"sv, true), illegal_string_error);
    EXPECT_THROW(verify_string("abc\0def"sv, false), illegal_string_error);
    EXPECT_THROW(verify_string("abc\ndef"sv, true), illegal_string_error);
    EXPECT_THROW(verify_string("abc:def"sv, true), illegal_string_error);
    EXPECT_THROW(verify_string("abc**def"sv, true), illegal_string_error);
    EXPECT_THROW(verify_string("abc?def"sv, true), illegal_string_error);
    EXPECT_THROW(verify_string("abc\"def"sv, true), illegal_string_error);
    EXPECT_THROW(verify_string("abc\\\\def"sv, true), illegal_string_error);
    verify_string("abc\\\\def"sv, false);
    EXPECT_THROW(verify_password("abc"sv), illegal_string_error);
    EXPECT_THROW(verify_password("abc\naaaaaaaaaaaaaa"sv), illegal_string_error);
    verify_password("abcd\\efgs"sv);
}
