#pragma once

#include "model/item.h"
#include <QAbstractItemModel>
#include <vector>

namespace hakurei::ui
{
using namespace core::model;
namespace model
{
class item_list_model : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit item_list_model(QWidget* parent);
    ~item_list_model() override = default;
    int rowCount(const QModelIndex& parent) const override { return _items.size(); }
    // Not supported, a QItemDelegated is required
    QVariant data(const QModelIndex& index, int role) const override { return QVariant(); }

    [[nodiscard]] const item* get_item(int row) const
    {
        if (row >= _items.size())
            return nullptr;
        return &_items[row];
    }

    void populate(std::vector<item> const& src_copy)
    {
        if (!_items.empty())
        {
            emit beginRemoveRows(QModelIndex(), 0, _items.size() - 1);
            _items.clear();
            emit endRemoveRows();
        }

        if (!src_copy.empty())
        {
            emit beginInsertRows(QModelIndex(), 0, src_copy.size() - 1);
            _items = src_copy;
            emit endInsertRows();
        }
    }
    void populate(std::vector<item>&& src_move)
    {
        if (!_items.empty())
        {
            emit beginRemoveRows(QModelIndex(), 0, _items.size() - 1);
            _items.clear();
            emit endRemoveRows();
        }

        if (!src_move.empty())
        {
            emit beginInsertRows(QModelIndex(), 0, src_move.size() - 1);
            _items = std::move(src_move);
            emit endInsertRows();
        }
    }

private:
    std::vector<item> _items;
};
}

}