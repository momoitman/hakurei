#include "serialization.h"

#include "internal_exceptions.h"
#include "model/item.h"
#include "model/user.h"
#include "model/order.h"
#include "../persistence/table.h"

#include <fmt/core.h>
#include <stdexcept>

namespace hakurei::core
{
namespace model
{
#define DESERIALIZE_FIELD(field, type, ordi)                                                                      \
    if (!std::holds_alternative<type>(row[ordi]))                                                                 \
        throw invalid_argument_error("Failed to serialize: Bad cell type in cell " #ordi ", " #type " expected"); \
    field = std::get<type>(row[ordi]);
#define DESERIALIZE_FIELD_CONV(field, type, conv, ordi)                                                           \
    if (!std::holds_alternative<type>(row[ordi]))                                                                 \
        throw invalid_argument_error("Failed to serialize: Bad cell type in cell " #ordi ", " #type " expected"); \
    field = conv(std::get<type>(row[ordi]));
#define DESERIALIZE_DATETIME_FIELD(field, ordi)                                                                  \
    if (!std::holds_alternative<int64_t>(row[ordi]))                                                             \
        throw invalid_argument_error("Failed to serialize: Bad cell type in cell " #ordi ", uint64_t expected"); \
    field = deserialize_datetime(std::get<int64_t>(row[ordi]));

void serializer::serialize(user const& obj, persistence::table::row_t& dest)
{
    dest.clear();
    dest.reserve(6);
    dest.emplace_back(obj._id);
    dest.emplace_back(obj._name);
    dest.emplace_back(obj._password_obfus.value_or(""));
    dest.emplace_back(obj._contact);
    dest.emplace_back(obj._address);
    dest.emplace_back(obj._balance_cents);
}

void serializer::serialize(item const& obj, persistence::table::row_t& dest)
{
    dest.clear();
    dest.reserve(7);
    dest.emplace_back(obj._id);
    dest.emplace_back(obj._name);
    dest.emplace_back(obj._price_cents);
    dest.emplace_back(obj._description);
    dest.emplace_back(obj._seller_uid);
    dest.emplace_back(serialize_datetime(obj._on_stock_time));
    dest.emplace_back(static_cast<int32_t>(obj._status));
}

void serializer::serialize(order const& obj, persistence::table::row_t& dest)
{
    dest.clear();
    dest.reserve(6);
    dest.emplace_back(obj._id);
    dest.emplace_back(obj._item_id);
    dest.emplace_back(obj._price_cents);
    dest.emplace_back(serialize_datetime(obj._time));
    dest.emplace_back(obj._seller_uid);
    dest.emplace_back(obj._customer_uid);
}

void serializer::deserialize(persistence::table::row_t const& row, user& dest)
{
    if (row.size() < 6)
        throw invalid_argument_error("Failed to deserialize: Wrong row size for type 'user': < 6");
    DESERIALIZE_FIELD(dest._id, std::string, 0)
    DESERIALIZE_FIELD(dest._name, std::string, 1)
    DESERIALIZE_FIELD(dest._password_obfus, std::string, 2)
    DESERIALIZE_FIELD(dest._contact, std::string, 3)
    DESERIALIZE_FIELD(dest._address, std::string, 4)
    DESERIALIZE_FIELD(dest._balance_cents, int, 5)
}

void serializer::deserialize(persistence::table::row_t const& row, item& dest)
{
    if (row.size() < 7)
        throw invalid_argument_error("Failed to deserialize: Wrong row size for type 'item': < 7");
    DESERIALIZE_FIELD(dest._id, std::string, 0)
    DESERIALIZE_FIELD(dest._name, std::string, 1)
    DESERIALIZE_FIELD(dest._price_cents, std::int32_t, 2)
    DESERIALIZE_FIELD(dest._description, std::string, 3)
    DESERIALIZE_FIELD(dest._seller_uid, std::string, 4)
    DESERIALIZE_DATETIME_FIELD(dest._on_stock_time, 5)
    DESERIALIZE_FIELD_CONV(dest._status, int32_t, deserialize_item_status, 6)
}

void serializer::deserialize(persistence::table::row_t const& row, order& dest)
{
    if (row.size() < 6)
        throw invalid_argument_error("Failed to deserialize: Wrong row size for type 'order': < 6");
    DESERIALIZE_FIELD(dest._id, std::string, 0)
    DESERIALIZE_FIELD(dest._item_id, std::string, 1)
    DESERIALIZE_FIELD(dest._price_cents, std::int32_t, 2)
    DESERIALIZE_DATETIME_FIELD(dest._time, 3)
    DESERIALIZE_FIELD(dest._seller_uid, std::string, 4)
    DESERIALIZE_FIELD(dest._customer_uid, std::string, 5)
}

item_status serializer::deserialize_item_status(int8_t val)
{
    if (val < 1 || val > 3)
        throw invalid_argument_error(fmt::format("Failed to deserialize: Invalid enum value {}", val));
    return static_cast<item_status>(val);
}

int64_t serializer::serialize_datetime(datetime_t value)
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(value.time_since_epoch()).count();
}

datetime_t serializer::deserialize_datetime(int64_t value)
{
    return std::chrono::system_clock::time_point(std::chrono::milliseconds(value));
}

const char* persistence_initializer<user>::name()
{
    return "user";
}

persistence::table::table_desc persistence_initializer<user>::table_desc()
{
    using namespace persistence::table;
    return persistence::table::table_desc(
        {"id", "name", "password", "contact", "address", "balance"},
        {table_column_type::string, table_column_type::string, table_column_type::string,
         table_column_type::string, table_column_type::string, table_column_type::int32}
    );
}

const char* persistence_initializer<order>::name()
{
    return "order";
}

persistence::table::table_desc persistence_initializer<order>::table_desc()
{
    using namespace persistence::table;
    return persistence::table::table_desc(
        {"id", "item_id", "price_cents", "time", "seller_uid", "customer_uid"},
        {table_column_type::string, table_column_type::string, table_column_type::int32,
         table_column_type::int64, table_column_type::string, table_column_type::string});
}

const char* persistence_initializer<item>::name()
{
    return "item";
}

persistence::table::table_desc persistence_initializer<item>::table_desc()
{
    using namespace persistence::table;
    return persistence::table::table_desc(
        {"id", "name", "price_cents", "description", "seller_uid", "on_stock_time", "status"},
        {table_column_type::string, table_column_type::string, table_column_type::int32,
         table_column_type::string, table_column_type::string,
            table_column_type::int64, table_column_type::int32},
        {1}); // index name only
}
}  // namespace model
}  // namespace hakurei::core