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
};
}
}