#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "service/id_generator.h"
#include "service/service_exceptions.h"

using namespace hakurei::core::service;

TEST(Hakurei_service_test, id_generator)
{
    EXPECT_EQ(generate_next_id("U00000"), "U00001");
    EXPECT_EQ(generate_next_id("M00010"), "M00011");
    EXPECT_EQ(generate_next_id("T10001"), "T10002");
    EXPECT_THROW(generate_next_id("AB0111"), id_gen_failed_error);
    EXPECT_THROW(generate_next_id("A-0001"), id_gen_failed_error);
    EXPECT_THROW(generate_next_id("T0000A"), id_gen_failed_error);
    EXPECT_THROW(generate_next_id("--0001"), id_gen_failed_error);
    EXPECT_THROW(generate_next_id("ABCD"), id_gen_failed_error);
    EXPECT_THROW(generate_next_id("1234"), id_gen_failed_error);
    EXPECT_THROW(generate_next_id("T001"), id_gen_failed_error);
    EXPECT_THROW(generate_next_id("T99999"), id_gen_failed_error);
}