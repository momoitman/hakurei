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
    if (!_table->rows().erase(id) > 0)
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
}  // namespace persistence
}  // namespace hakurei::core