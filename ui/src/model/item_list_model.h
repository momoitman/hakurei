#pragma once

#include "model/item.h"
#include "model/simple_model_holder.h"
#include <QAbstractItemModel>
#include <vector>

namespace hakurei::ui
{
using namespace core::model;
namespace model
{
class item_list_model : public QAbstractListModel, public simple_model_holder<item, item_list_model>
{
    Q_OBJECT
public:
    friend class simple_model_holder<item, item_list_model>;
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

    [[nodiscard]] const item* get_item(const QModelIndex& index) const
    {
        return get_item(index.row());
    }

    void populate2(std::vector<item> const& src_copy)
    {
        if (!_items.empty())
        {
            beginRemoveRows(QModelIndex(), 0, _items.size() - 1);
            _items.clear();
            endRemoveRows();
        }

        if (!src_copy.empty())
        {
            beginInsertRows(QModelIndex(), 0, src_copy.size() - 1);
            _items = src_copy;
            endInsertRows();
        }
    }

    void populate2(std::vector<item>&& src_move)
    {
        if (!_items.empty())
        {
            this->beginRemoveRows(QModelIndex(), 0, _items.size() - 1);
            _items.clear();
            this->endRemoveRows();
        }

        if (!src_move.empty())
        {
            this->beginInsertRows(QModelIndex(), 0, src_move.size() - 1);
            _items = std::move(src_move);
            this->endInsertRows();
        }
    }
public:
    //std::vector<item> _items;
};
}

}