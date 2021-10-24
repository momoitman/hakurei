#pragma once

#include <QStyledItemDelegate>
namespace hakurei::ui
{
namespace model
{

class two_precision_cell_delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit two_precision_cell_delegate(QObject* obj)
        : QStyledItemDelegate(obj) {}
    ~two_precision_cell_delegate() override = default;
    QString displayText(const QVariant& value, const QLocale& locale) const override;
};

class datetime_cell_delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit datetime_cell_delegate(QObject* obj)
        : QStyledItemDelegate(obj) {}
    ~datetime_cell_delegate() override = default;
    QString displayText(const QVariant& value, const QLocale& locale) const override;
};
}
}  // namespace hakurei::ui