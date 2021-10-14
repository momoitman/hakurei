#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "persistence/table.h"
#include "persistence_test.h"

using namespace hakurei::core::persistence;
using namespace hakurei::core;

TEST(Hakurei_persistence_test, table_desc)
{
    {
        auto desc = create_test_table_desc();
        EXPECT_EQ(desc.column_names().size(), 2);
        EXPECT_EQ(desc.column_names()[0], "id");
        EXPECT_EQ(desc.column_names()[1], "name");
    }
    EXPECT_THROW(
        {
            table::table_desc desc_bad1(
                {"id"},
                {table::table_column_type::string, table::table_column_type::string});
        },
        invalid_argument_error);
    EXPECT_THROW(
        {
            table::table_desc desc_bad2({}, {});
        },
        invalid_argument_error);
}

TEST(Hakurei_persistence_test, table_cell_to_string)
{
    // ReSharper disable CppFunctionalStyleCast
    table::cell_t cell = int32_t(10);
    EXPECT_EQ(table::to_string(cell), "10");

    cell = int64_t(100000000000);
    EXPECT_EQ(table::to_string(cell), "100000000000");

    cell = double(3.1415926);
    EXPECT_EQ(table::to_string(cell), "3.14");
    EXPECT_EQ(table::to_string(cell, 7), "3.1415926");

    cell = std::string("test_!");
    EXPECT_EQ(table::to_string(cell), "test_!");
    // ReSharper restore CppFunctionalStyleCast
}

TEST(Hakurei_persistence_test, table)
{
    table::table table(create_test_table_desc());
    table::row_t test_row(std::initializer_list<table::cell_t>({"15", "Test"}));

    table.check_row_fit(test_row);
    table.rows().emplace("15", test_row);
    EXPECT_EQ(table.size(), 1);
    EXPECT_THAT(table.rows()["15"], testing::ElementsAre("15", "Test"));
    table.rows().erase(table.rows().find("15"));
    EXPECT_EQ(table.size(), 0);

    table::row_t test_row_bad1(std::initializer_list<table::cell_t>({"15"}));
    EXPECT_THROW(table.check_row_fit(test_row_bad1), invalid_argument_error);
    table::row_t test_row_bad2(std::initializer_list<table::cell_t>({"15", 15}));
    EXPECT_DEATH(table.check_row_fit(test_row_bad2), "check_row_fit");
}
