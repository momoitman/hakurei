#pragma once

#include "../persistence/table.h"
#include "model/model_base.h"

namespace hakurei::core
{
namespace model
{

class user;
class item;
class order;
enum class item_status : uint8_t;

struct serializer
{
    static void serialize(user const& obj, persistence::table::row_t& dest);
    static void serialize(item const& obj, persistence::table::row_t& dest);
    static void serialize(order const& obj, persistence::table::row_t& dest);
    static void deserialize(persistence::table::row_t const& row, user& dest);
    static void deserialize(persistence::table::row_t const& row, item& dest);
    static void deserialize(persistence::table::row_t const& row, order& dest);
    static item_status deserialize_item_status(int8_t val);

    static int64_t serialize_datetime(datetime_t value);
    static datetime_t deserialize_datetime(int64_t value);
};

}
}  // namespace hakurei::core