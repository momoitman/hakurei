#include "order_table_model.h"

namespace hakurei::ui
{
namespace model
{
order_table_model::order_table_model(QObject* parent)
    : QAbstractTableModel(parent)
{
}

QVariant order_table_model::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    using namespace std::chrono;
    if (index.row() >= _items.size())
        return QVariant();
    auto& it = _items[index.row()];
    switch (index.column())
    {
    case 0:
        return QVariant(QString::fromStdString(it.id()));
    case 1:
        return QVariant(QString::fromStdString(it.item_id()));
    case 2:
        return QVariant(it.price_cents() / 100.0);
    case 3:
        return QVariant::fromValue(duration_cast<milliseconds>(it.time().time_since_epoch()).count());
    case 4:
        return QVariant(QString::fromStdString(it.seller_uid()));
    case 5:
        return QVariant(QString::fromStdString(it.customer_uid()));
    }
}

QVariant order_table_model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();
    switch (section)
    {
    case 0:
        return tr("ID");
    case 1:
        return tr("商品 ID");
    case 2:
        return tr("价格");
    case 3:
        return tr("订单时间");
    case 4:
        return tr("卖家");
    case 5:
        return tr("买家");
    }
    return QVariant();
}

int order_table_model::column_index_price_cents()
{
    return 2;
}

int order_table_model::column_index_time()
{
    return 3;
}
}
}  // namespace hakurei::ui