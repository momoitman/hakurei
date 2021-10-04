#pragma once

#include "model/item.h"
#include "model/order.h"
#include "model/user.h"
#include "model/serialization.h"
#include "persistence/abstract_persistence.h"
#include "util/setting.h"

#include <tuple>
#include <memory>

namespace hakurei::core
{
namespace model
{
template<class T, class Id>
class repository
{
public:
    repository()
        : _persistence(persistence::get_active_persistence_registry().construct(
            get_active_setting()["persistence"]["persistence_provider"].value_or(""),
                persistence_initializer<T>::name(),
                persistence_initializer<T>::table_desc()
        ))
    {
        _persistence->load();
    }

    bool save(T const& obj)
    {
        serializer::serialize(obj, _temp_row);
        return _persistence->save(_temp_row);
    }
    bool load(Id const& id, T& dest)
    {
        if (!_persistence->load(id, _temp_row))
            return false;
        serializer::deserialize(_temp_row, dest);
        return true;
    }

    template <class Crit>
    void find_by_column(int column_index, Crit const& criteria, std::vector<T>& dest)
    {
        _persistence->find_by_column(
            column_index, persistence::table::cell_t(criteria), _temp_rows);
        dest.clear();
        dest.resize(_temp_rows.size());
        for (size_t i = 0; i < _temp_rows.size(); ++i)
            serializer::deserialize(_temp_rows[i], dest[i]);
    }

    template <class Crit>
    bool find_one_by_column(int column_index, Crit const& criteria, T& dest)
    {
        if (!_persistence->find_one_by_column(
            column_index, persistence::table::cell_t(criteria), _temp_row
        ))
            return false;
        serializer::deserialize(_temp_row, dest);
        return true;
    }

    void flush() { _persistence->save(); }
    size_t size() { return _persistence->size(); }

    bool remove(Id const& id) { return _persistence->remove(persistence::table::cell_t(id)); }
    bool append(T& obj)
    {
        serializer::serialize(obj, _temp_row);
        return _persistence->save(_temp_row);
    }

    std::optional<Id> get_last_id()
    {
        auto opt = _persistence->get_last_id();
        if (!opt)
            return std::nullopt;
        return std::get<Id>(opt.value());
    }

private:
    persistence::table::row_t _temp_row;
    std::vector<persistence::table::row_t> _temp_rows;
    std::unique_ptr<persistence::abstract_persistence> _persistence;
};

using repository_hub = 
    std::tuple< 
        repository<user, std::string>,
        repository<item, std::string>,
        repository<order, std::string>
    >;

std::shared_ptr<repository_hub> get_active_repositories();
}
}  // namespace hakurei::core