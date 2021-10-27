#pragma once

#include "model/model_base.h"
#include <QString>
#include <QDateTime>

namespace hakurei::ui
{
inline QString arg_price_cents(QString const& str, int price_cents)
{
    return str.arg(price_cents / 100.0, 0, 'f', 2);
}

inline QString to_string_price(double price)
{
    return QString::number(price, 'f', 2);
}

inline QString to_string_price_cents(int price_cents)
{
    return to_string_price(price_cents / 100.0);
}

inline QString to_string_datetime(uint64_t t)
{
    return QDateTime::fromMSecsSinceEpoch(t).toString("yyyy-MM-dd hh:mm:ss");
}

inline QString to_string_datetime(core::model::datetime_t t)
{
    return to_string_datetime(std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count());
}
}