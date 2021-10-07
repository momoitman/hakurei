#include "abstract_persistence.h"

namespace hakurei::core
{
namespace persistence
{
bool abstract_memory_persistence::save(table::row_t const& row)
{
    _table->check_row_fit(row);
    auto [_, inserted] = _table->rows().insert_or_assign(row[0], row);
    save();
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
    return true;
}

bool abstract_memory_persistence::append(table::row_t& row)
{
    _table->check_row_fit(row);
    auto [_, inserted] = _table->rows().try_emplace(row[0], row);
    if (inserted)
        save();
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
            });
}
}  // namespace persistence
}  // namespace hakurei::core