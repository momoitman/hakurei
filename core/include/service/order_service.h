#pragma once

#include "model/item.h"
#include "model/order.h"
#include "service/auth_service.h"

#include <vector>

namespace hakurei::core
{
namespace service
{
class order_service
{
public:
    virtual ~order_service() = default;
    virtual void place_order(auth_token token, std::string_view item_id) = 0;
    virtual void get_my_bought_orders(auth_token token, std::vector<model::order>& dest) = 0;
    virtual void get_my_sold_orders(auth_token token, std::vector<model::order>& dest) = 0;
    virtual void get_all_orders(auth_token token, std::vector<model::order>& dest) = 0;
};
}
}  // namespace hakurei::core