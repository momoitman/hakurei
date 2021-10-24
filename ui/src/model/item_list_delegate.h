#pragma once

#include <QItemDelegate>

namespace hakurei::ui
{
class item_list_delegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit item_list_delegate(QWidget* parent) : QItemDelegate(parent) {  }
    ~item_list_delegate() override = default;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};
}