#pragma once

#include "table.h"
#include "util/factory_registry.h"
#include "util/setting.h"

#include <string>
#include <memory>
#include <stdexcept>
#include <functional>
#include <fruit/fruit.h>

namespace hakurei::core
{
namespace persistence
{
class abstract_persistence
{
public:
    virtual bool save(table::row_t const& row) = 0;
    virtual bool load(table::cell_t const& id, table::row_t& row) = 0;
    virtual void find_by_column(int column_idx, table::cell_t const& content, std::vector<table::row_t>& dest) = 0;
    virtual bool find_one_by_column(int column_idx, table::cell_t const& content, table::row_t& dest) = 0;

    virtual void save() = 0;
    virtual void load() = 0;
    virtual size_t size() = 0;

    virtual bool remove(table::cell_t const& id) = 0;
    virtual bool append(table::row_t& row) = 0;
    virtual std::optional<table::cell_t> get_last_id() = 0;

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
    std::optional<table::cell_t> get_last_id() override
    {
        auto it = _table->rows().rbegin();
        if (it == _table->rows().rend())
            return std::nullopt;
        return it->first;
    }
    void find_by_column(int column_idx, table::cell_t const& content,
        std::vector<table::row_t>& dest) override
    {
        // TODO: optimize with boost::multiindex
        if (column_idx >= _table->desc().size())
            throw std::invalid_argument("Bad column index");
        auto& rows = _table->rows();
        dest.clear();
        for (auto& [_, row] : rows)
            if (row[column_idx] == content)
                dest.push_back(row);
    }
    bool find_one_by_column(int column_idx, table::cell_t const& content,
                            table::row_t& dest) override
    {
        // TODO: optimize with boost::multiindex
        if (column_idx >= _table->desc().size())
            throw std::invalid_argument("Bad column index");
        auto& rows = _table->rows();
        for (auto& [_, row] : rows)
            if (row[column_idx] == content)
            {
                dest = row;
                return true;
            }
        return false;
    }

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

using persistence_factory_registry = factory_registry<
    std::unique_ptr<abstract_persistence>,
    std::unique_ptr<abstract_persistence> (*)(setting_t const&, std::string const&, table::table_desc const&),
    setting_t const&, std::string, table::table_desc
>;
persistence_factory_registry& get_persistence_registry();

using persistence_factory
    = std::function<std::unique_ptr<abstract_persistence>(std::string const&, table::table_desc const&)>;
using persistence_component = fruit::Component<fruit::Required<setting_t>, persistence_factory>;
persistence_component get_persistence_component();

}  // namespace persistence
}  // namespace hakurei::core