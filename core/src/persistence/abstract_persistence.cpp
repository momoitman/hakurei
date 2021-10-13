#include "abstract_persistence.h"

#include <utility>

namespace hakurei::core
{
namespace persistence
{
bool abstract_memory_persistence::save(table::row_t const& row)
{
    _table->check_row_fit(row);
    auto [_, inserted] = _table->rows().insert_or_assign(row[0], row);
    save();
    if (inserted)
        _search_eng->notify_append(row);
    else
        _search_eng->notify_change(row);
    return inserted;
}

bool abstract_memory_persistence::load(table::cell_t const& id, table::row_t& row)
{
    auto& rows = _table->rows();
    auto iter = rows.find(id);
    if (iter == rows.end())
        return false;
    row = iter->second;
    return true;
}

bool abstract_memory_persistence::remove(table::cell_t const& id)
{
    if (_table->rows().erase(id) <= 0)
        return false;
    save();
    _search_eng->notify_delete(id);
    return true;
}

bool abstract_memory_persistence::append(table::row_t& row)
{
    _table->check_row_fit(row);
    auto [_, inserted] = _table->rows().try_emplace(row[0], row);
    if (inserted)
    {
        save();
        _search_eng->notify_append(row);
    }
    return inserted;
}

persistence_factory_registry default_registry;
persistence_factory_registry& get_persistence_registry()
{
    return default_registry;
}

using persistence_factory_lambda = std::unique_ptr<abstract_persistence>(
    setting_t const&, 
    fruit::Assisted<std::string const&>, 
    fruit::Assisted<table::table_desc const&>);
using search_engine_factory_lambda = std::unique_ptr<abstract_search_engine>(setting_t const&);

persistence_component get_persistence_component()
{
    return fruit::createComponent()
        .registerFactory<persistence_factory_lambda>(
            [](setting_t const& setting,
               std::string const& name, table::table_desc const& desc)
                -> std::unique_ptr<abstract_persistence>
            {
                return get_persistence_registry().construct(
                    setting["persistence"]["persistence_provider"].value_or(""),
                    setting, name, desc);
            })
        .registerFactory<search_engine_factory_lambda>(
            [](setting_t const& setting)
                -> std::unique_ptr<abstract_search_engine>
            {
                return get_search_engine_registry().construct(
                    setting["persistence"]["search_engine_provider"].value_or(""),
                    setting
                );
            });
}
}  // namespace persistence
}  // namespace hakurei::core