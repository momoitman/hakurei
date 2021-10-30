#pragma once

#include "model/item.h"
#include "model/order.h"
#include "simple_model_holder.h"
#include <QAbstractTableModel>

namespace hakurei::ui
{
namespace model
{
using namespace core::model;
class order_table_model : public QAbstractTableModel, public simple_model_holder<order, order_table_model>
{
    Q_OBJECT
public:
    friend class simple_model_holder<order, order_table_model>;
    static constexpr int column_id = 0;
    static constexpr int column_item_id = 1;
    static constexpr int column_price_cents = 2;
    static constexpr int column_time = 3;
    static constexpr int column_seller_id = 4;
    static constexpr int column_customer_id = 5;

    explicit order_table_model(QObject* parent);
    ~order_table_model() override = default;
    int rowCount(const QModelIndex& parent) const override { return _items.size(); }
    int columnCount(const QModelIndex& parent) const override { return 6; }
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};

class item_table_model : public QAbstractTableModel, public simple_model_holder<item, item_table_model>
{
    Q_OBJECT
public:
    friend class simple_model_holder<item, item_table_model>;
    static constexpr int column_id = 0;
    static constexpr int column_name = 1;
    static constexpr int column_price_cents = 2;
    static constexpr int column_description = 3;
    static constexpr int column_seller_uid = 4;
    static constexpr int column_on_stock_time = 5;
    static constexpr int column_status = 6;

    explicit item_table_model(QObject* parent);
    ~item_table_model() override = default;
    int rowCount(const QModelIndex& parent) const override { return _items.size(); }
    int columnCount(const QModelIndex& parent) const override { return 7; }
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
}
}  // namespace hakurei::ui