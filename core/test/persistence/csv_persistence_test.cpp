#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "persistence/csv_persistence.h"
#include "persistence_test.h"

#include <filesystem>
#include <fstream>
#include <string_view>

using namespace hakurei::core::persistence;

std::string read_csv(std::filesystem::path path)
{
    std::ifstream ifs;
    ifs.exceptions(std::ios_base::badbit | std::ios_base::failbit);
    ifs.open(path);
    ifs.seekg(0, std::ios::end);
    size_t size = ifs.tellg();
    std::string buffer(size, ' ');
    ifs.seekg(0);
    ifs.read(&buffer[0], size);
    return buffer;
}

TEST(Hakurei_persistence_test, csv_persistence)
{
    std::filesystem::path base_path("test_data");
    std::error_code ec; // IGNORED
    std::filesystem::remove_all(base_path, ec);
    csv_persistence pers("test", create_test_table_desc(), base_path, std::make_unique<mock_search_engine>());

    table::row_t row1(std::initializer_list<table::cell_t>{"10", "te\\st10\"\n\"\""});
    table::row_t row2(std::initializer_list<table::cell_t>{"5", "\"test5\""});

    EXPECT_TRUE(pers.append(row1));
    EXPECT_TRUE(pers.append(row2));
    EXPECT_FALSE(pers.append(row1));

    auto contents = read_csv(base_path / "test.csv");
    EXPECT_EQ(contents, 
        R"eof(id,name
10,"te\st10""
"""""
5,"""test5"""
)eof"
    );

    csv_persistence pers2("test", create_test_table_desc(), base_path, std::make_unique<mock_search_engine>());
    pers2.load();
    EXPECT_EQ(pers2.size(), 2);
    table::row_t row3, row4;
    EXPECT_TRUE(pers.load("10", row3));
    EXPECT_TRUE(pers.load("5", row4));
    EXPECT_THAT(row3, testing::ElementsAre("10", "te\\st10\"\n\"\""));
    EXPECT_THAT(row4, testing::ElementsAre("5", "\"test5\""));
}
