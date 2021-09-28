#pragma once

#include "table.h"

#include <string>
#include <memory>
#include <stdexcept>

namespace hakurei::core
{
namespace persistence
{
class abstract_persistence
{
public:
    virtual bool save(table::row_t const& row) = 0;
    virtual bool load(table::cell_t const& id, table::row_t& row) = 0;

    virtual void save() = 0;
    virtual void load() = 0;
    virtual size_t size() = 0;

    virtual bool remove(table::cell_t const& id) = 0;
    virtual bool append(table::row_t& row) = 0;

    /**
     * Get all rows in the form of a table.
     * If the data isn't directly stored in the persistence object,
     * rows will be retrieved and stored in a new table object.
     */
    virtual std::shared_ptr<table::table> all_rows_table() = 0;

    abstract_persistence(std::string name, table::table_desc const& table_spec)
        : _name(std::move(name)), _table_desc(table_spec) {}
    virtual ~abstract_persistence() {};

    abstract_persistence& operator=(const abstract_persistence& other) = delete;
    abstract_persistence& operator=(abstract_persistence&& other) noexcept = delete;

protected:
    std::string _name;
    table::table_desc _table_desc;
};

class abstract_memory_persistence : public abstract_persistence
{
public:
    using abstract_persistence::save;
    using abstract_persistence::load;
    using abstract_persistence::size;
    size_t size() override { return _table->size(); }
    bool save(table::row_t const& row) override;
    bool load(table::cell_t const& id, table::row_t& row) override;
    bool remove(table::cell_t const& id) override;
    bool append(table::row_t& row) override;
    std::shared_ptr<table::table> all_rows_table() override { return _table; }

    abstract_memory_persistence(std::string const& name, table::table_desc const& table_spec)
        : abstract_persistence(name, table_spec),
          _table(std::make_shared<table::table>(table_spec))
    {
    }

    ~abstract_memory_persistence() override {};

protected:
    std::shared_ptr<table::table> _table;
};

class persistence_error : public std::runtime_error
{
public:
    explicit persistence_error(const std::string& basic_string)
        : runtime_error(basic_string)
    {
    }

    explicit persistence_error(const char* string)
        : runtime_error(string)
    {
    }
};

}  // namespace persistence
}  // namespace hakurei::core