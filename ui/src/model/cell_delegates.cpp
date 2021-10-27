#include "cell_delegates.h"

#include "util/str_util.h"

namespace hakurei::ui
{
namespace model
{
QString two_precision_cell_delegate::displayText(const QVariant& value, const QLocale& locale) const
{
    return to_string_price(value.toDouble());
}

QString datetime_cell_delegate::displayText(const QVariant& value, const QLocale& locale) const
{
    return to_string_datetime(value.toULongLong());
}
}
}  // namespace hakurei::ui