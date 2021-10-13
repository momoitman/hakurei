#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "persistence/abstract_persistence.h"

using namespace hakurei::core::persistence;

class mock_search_engine : public abstract_search_engine
{
public:
    ~mock_search_engine() = default;
    void attach_persistence(abstract_persistence*, std::vector<int> columns) {}
    void build_index() {}
    void search(std::string_view keywords, std::vector<table::row_t>& dest) {}
    void notify_change(table::row_t const& row) {}
    void notify_delete(table::cell_t const& id) {}
    void notify_append(table::row_t const& row) {}
};

class mock_memory_persistence : public abstract_memory_persistence
{
public:
    using abstract_persistence::save;
    using abstract_persistence::load;
    MOCK_METHOD(void, save, (), (override));
    MOCK_METHOD(void, load, (), (override));

    mock_memory_persistence(std::string const& name, table::table_desc const& table_spec)
        : abstract_memory_persistence(name, table_spec, std::make_unique<mock_search_engine>()) {}
};

inline table::table_desc create_test_table_desc()
{
    return table::table_desc(
        {"id", "name"},
        {table::table_column_type::string, table::table_column_type::string});
}