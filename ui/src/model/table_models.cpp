#include "table_models.h"

#include <QHeaderView>

#include "cell_delegates.h"

namespace hakurei::ui
{
namespace model
{
order_table_model::order_table_model(QObject* parent)
    : QAbstractTableModel(parent), simple_model_holder(this)
{
}

QVariant order_table_model::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole || index.row() >= _items.size())
        return QVariant();

    using namespace std::chrono;
    auto& it = _items[index.row()];
    switch (index.column())
    {
    case column_id:
        return QVariant(QString::fromStdString(it.id()));
    case column_item_id:
        return QVariant(QString::fromStdString(it.item_id()));
    case column_price_cents:
        return QVariant(it.price_cents() / 100.0);
    case column_time:
        return QVariant::fromValue(duration_cast<milliseconds>(it.time().time_since_epoch()).count());
    case column_seller_id:
        return QVariant(QString::fromStdString(it.seller_uid()));
    case column_customer_id:
        return QVariant(QString::fromStdString(it.customer_uid()));
    }
    return QVariant();
}

QVariant order_table_model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();
    switch (section)
    {
    case column_id:
        return tr("ID");
    case column_item_id:
        return tr("商品 ID");
    case column_price_cents:
        return tr("价格");
    case column_time:
        return tr("订单时间");
    case column_seller_id:
        return tr("卖家");
    case column_customer_id:
        return tr("买家");
    }
    return QVariant();
}

void order_table_model::configure_table(QTableView* view)
{
    view->horizontalHeader()->resizeSection(0, 100);
    view->horizontalHeader()->resizeSection(1, 100);
    view->horizontalHeader()->resizeSection(2, 100);
    view->horizontalHeader()->resizeSection(3, 200);
    view->horizontalHeader()->resizeSection(4, 100);
    view->horizontalHeader()->resizeSection(5, 100);
    view->setItemDelegateForColumn(
        column_time,
        new model::datetime_cell_delegate(view));
    view->setItemDelegateForColumn(
        column_price_cents,
        new model::two_precision_cell_delegate(view));
}

item_table_model::item_table_model(QObject* parent)
    : QAbstractTableModel(parent), simple_model_holder(this)
{
}

QVariant item_table_model::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole || index.row() >= _items.size())
        return QVariant();

    using namespace std::chrono;
    auto& it = _items[index.row()];
    switch (index.column())
    {
    case column_id:
        return QVariant(QString::fromStdString(it.id()));
    case column_name:
        return QVariant(QString::fromStdString(it.name()));
    case column_price_cents:
        return QVariant(it.price_cents() / 100.0);
    case column_description:
        return QVariant(QString::fromStdString(it.description()));
    case column_seller_uid:
        return QVariant(QString::fromStdString(it.seller_uid()));
    case column_on_stock_time:
        return QVariant::fromValue(duration_cast<milliseconds>(it.on_stock_time().time_since_epoch()).count());
    case column_status:
        switch (it.status())
        {
        case item_status::on_stock:
            return "在售";
        case item_status::sold:
            return "已售出";
        case item_status::deleted:
            return "已下架";
        }
    }
    return QVariant();
}

QVariant item_table_model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();
    switch (section)
    {
    case column_id:
        return tr("ID");
    case column_name:
        return tr("名称");
    case column_price_cents:
        return tr("价格");
    case column_description:
        return tr("描述");
    case column_seller_uid:
        return tr("卖家");
    case column_on_stock_time:
        return tr("上架时间");
    case column_status:
        return tr("状态");
    }
    return QVariant();
}

void item_table_model::configure_table(QTableView* view)
{
    view->horizontalHeader()->resizeSection(0, 80);
    view->horizontalHeader()->resizeSection(1, 150);
    view->horizontalHeader()->resizeSection(2, 100);
    view->horizontalHeader()->resizeSection(3, 100);
    view->horizontalHeader()->resizeSection(4, 70);
    view->horizontalHeader()->resizeSection(5, 200);
    view->horizontalHeader()->resizeSection(6, 70);
    view->setItemDelegateForColumn(
        column_on_stock_time,
        new model::datetime_cell_delegate(view));
    view->setItemDelegateForColumn(
        column_price_cents,
        new model::two_precision_cell_delegate(view));
}

user_table_model::user_table_model(QObject* parent)
    : QAbstractTableModel(parent), simple_model_holder(this)
{
}

QVariant user_table_model::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole || index.row() >= _items.size())
        return QVariant();

    using namespace std::chrono;
    auto& it = _items[index.row()];
    switch (index.column())
    {
    case column_id:
        return QVariant(QString::fromStdString(it.id()));
    case column_name:
        return QVariant(QString::fromStdString(it.name()));
    case column_contact:
        return QVariant(QString::fromStdString(it.contact()));
    case column_address:
        return QVariant(QString::fromStdString(it.address()));
    case column_balance_cents:
        return QVariant(it.balance_cents() / 100.0);
    }
    return QVariant();
}

QVariant user_table_model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();
    switch (section)
    {
    case column_id:
        return tr("ID");
    case column_name:
        return tr("用户名");
    case column_contact:
        return tr("联系方式");
    case column_address:
        return tr("联系地址");
    case column_balance_cents:
        return tr("余额");
    }
    return QVariant();
}

void user_table_model::configure_table(QTableView* view)
{
    view->horizontalHeader()->resizeSection(0, 100);
    view->horizontalHeader()->resizeSection(1, 200);
    view->horizontalHeader()->resizeSection(2, 300);
    view->horizontalHeader()->resizeSection(3, 300);
    view->horizontalHeader()->resizeSection(4, 100);
    view->setItemDelegateForColumn(
        column_balance_cents,
        new model::two_precision_cell_delegate(view));
}
}
}  // namespace hakurei::ui