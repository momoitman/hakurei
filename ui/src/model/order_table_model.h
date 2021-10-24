#pragma once

#include "model/order.h"
#include <QAbstractTableModel>

namespace hakurei::ui
{
namespace model
{
using namespace core::model;
class order_table_model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit order_table_model(QObject* parent);
    ~order_table_model() override = default;
    int rowCount(const QModelIndex& parent) const override { return _items.size(); }
    int columnCount(const QModelIndex& parent) const override { return 6; }
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int column_index_price_cents();
    int column_index_time();

    void populate(std::vector<order> const& src_copy)
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
    void populate(std::vector<order>&& src_move)
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
    std::vector<order> _items;
};
}
}  // namespace hakurei::ui