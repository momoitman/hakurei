#include "cell_delegates.h"

#include <QDateTime>

namespace hakurei::ui
{
namespace model
{
QString two_precision_cell_delegate::displayText(const QVariant& value, const QLocale& locale) const
{
    return QString::number(value.toDouble(), 'f', 2);
}

QString datetime_cell_delegate::displayText(const QVariant& value, const QLocale& locale) const
{
    return QDateTime::fromMSecsSinceEpoch(value.toULongLong()).toString("yyyy-MM-dd hh:mm:ss");
}
}
}  // namespace hakurei::ui