#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "util/argon2_password_hasher.h"

using namespace std::literals;
using namespace hakurei::core::util;
using namespace hakurei::core;

TEST(Hakurei_util_test, argon2_password)
{
    argon2_password_hasher hasher;
    auto hash = hasher.hash_password("abcdef"sv);
    auto hash_bad = "ssssssssssssss";
    auto hash_bad2 = std::string(512, 'b');

    EXPECT_TRUE(hasher.verity_password("abcdef"sv, hash));
    EXPECT_FALSE(hasher.verity_password("123456"sv, hash));
    EXPECT_THROW(hasher.verity_password("abcdef"sv, hash_bad), password_hash_error);
    EXPECT_THROW(hasher.verity_password("abcdef"sv, hash_bad2), password_hash_error);
}