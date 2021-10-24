#pragma once

#include "model/item.h"
#include "model/order.h"
#include "model/user.h"
#include "model/serialization.h"
#include "persistence/abstract_persistence.h"
#include "util/setting.h"

#include <tuple>
#include <memory>
#include <fruit/fruit.h>

namespace hakurei::core
{
namespace model
{
template<class T, class Id>
class repository
{
public:
    repository(const setting_t& setting, persistence::persistence_factory persistence_factory)
        : _persistence(persistence_factory(persistence_initializer<T>::name(), persistence_initializer<T>::table_desc()))
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

    void get_all(std::vector<T>& dest)
    {
        auto const& rows = _persistence->all_rows_table()->rows();
        dest.resize(rows.size());
        size_t i = 0;
        for (auto const& [_, row] : rows)
        {
            serializer::deserialize(row, dest[i]);
            i++;
        }
    }

    void temp_rows_to_dest(std::vector<T>& dest)
    {
        dest.clear();
        dest.resize(_temp_rows.size());
        for (size_t i = 0; i < _temp_rows.size(); ++i)
            serializer::deserialize(_temp_rows[i], dest[i]);
    }

    template <class Crit>
    void find_by_column(int column_index, Crit const& criteria, std::vector<T>& dest)
    {
        _persistence->find_by_column(
            column_index, persistence::table::cell_t(criteria), _temp_rows);
        temp_rows_to_dest(dest);
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
    
    void search(std::string_view keyword, std::vector<T>& dest)
    {
        _persistence->search(keyword, _temp_rows);
        temp_rows_to_dest(dest);
    }

    void flush() { _persistence->save(); }
    size_t size() { return _persistence->size(); }

    bool remove(Id const& id) { return _persistence->remove(persistence::table::cell_t(id)); }
    bool append(T const& obj)
    {
        serializer::serialize(obj, _temp_row);
        return _persistence->append(_temp_row);
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

using user_repository = repository<class user, std::string>;
using item_repository = repository<class item, std::string>;
using order_repository = repository<class order, std::string>;

class repository_hub
{
public:
    INJECT(repository_hub(setting_t const& setting, persistence::persistence_factory p_factory))
        : _user(setting, p_factory), _item(setting, p_factory), _order(setting, p_factory)
    {}

    [[nodiscard]] repository<user, std::string>& user_repo() { return _user; }
    [[nodiscard]] repository<item, std::string>& item_repo() { return _item; }
    [[nodiscard]] repository<order, std::string>& order_repo() { return _order; }

private:
    user_repository _user;
    item_repository _item;
    order_repository _order;
};

using repository_component = fruit::Component<
    fruit::Required<setting_t>,
    repository_hub, persistence::persistence_factory>;

repository_component get_repository_component();
}
}  // namespace hakurei::core