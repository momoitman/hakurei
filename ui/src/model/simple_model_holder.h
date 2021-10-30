#pragma once
#include <vector>
#include <QAbstractItemModel>

namespace hakurei::ui
{

template <class T, class This>
class simple_model_holder
{
public:
    simple_model_holder(This* t): _this(t){}

    void populate(std::vector<T> const& src_copy)
    {
        if (!_items.empty())
        {
            _this-> beginRemoveRows(QModelIndex(), 0, _items.size() - 1);
            _items.clear();
            _this-> endRemoveRows();
        }

        if (!src_copy.empty())
        {
            _this-> beginInsertRows(QModelIndex(), 0, src_copy.size() - 1);
            _items = src_copy;
            _this-> endInsertRows();
        }
    }
    void populate(std::vector<T>&& src_move)
    {
        if (!_items.empty())
        {
            _this-> beginRemoveRows(QModelIndex(), 0, _items.size() - 1);
            _items.clear();
            _this-> endRemoveRows();
        }

        if (!src_move.empty())
        {
            _this-> beginInsertRows(QModelIndex(), 0, src_move.size() - 1);
            _items = std::move(src_move);
            _this-> endInsertRows();
        }
    }

    [[nodiscard]] const T* get_item(int row) const
    {
        if (row >= _items.size())
            return nullptr;
        return &_items[row];
    }

    [[nodiscard]] const T* get_item(const QModelIndex& index) const
    {
        return get_item(index.row());
    }

protected:
    This* _this;
    std::vector<T> _items;
};

}