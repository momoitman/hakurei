#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "persistence/abstract_persistence.h"
#include "persistence_test.h"

using namespace hakurei::core::persistence;

TEST(Hakurei_persistence_test, memory_persistence)
{
    mock_memory_persistence pers("test", create_test_table_desc());

    table::row_t row1(std::initializer_list<table::cell_t>{"10", "test10"});
    table::row_t row2(std::initializer_list<table::cell_t>{"5", "test5"});

    EXPECT_CALL(pers, save()).Times(0);
    EXPECT_FALSE(pers.load("10", row1));
    EXPECT_FALSE(pers.remove("10"));

    EXPECT_CALL(pers, save()).Times(2);
    EXPECT_TRUE(pers.append(row1));
    EXPECT_TRUE(pers.append(row2));
    EXPECT_EQ(pers.all_rows_table()->size(), 2);
    EXPECT_CALL(pers, save()).Times(0);
    EXPECT_FALSE(pers.append(row1));

    table::row_t row3;
    EXPECT_TRUE(pers.load("10", row3));
    EXPECT_THAT(row3, testing::ElementsAre("10", "test10"));
    row1[1] = "test20";
    EXPECT_CALL(pers, save()).Times(1);
    EXPECT_FALSE(pers.save(row1));
    EXPECT_TRUE(pers.load("10", row3));
    EXPECT_THAT(row3, testing::ElementsAre("10", "test20"));
}
