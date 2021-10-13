#pragma once

#include "service/order_service.h"
#include "model/repository.h"
#include "auth_service_intl.h"

#include <fruit/fruit.h>

namespace hakurei::core
{
namespace service
{
class item_service_impl;

class order_service_intl : public order_service
{
    // no internal methods yet
public:
};

class order_service_impl : public order_service_intl
{
public:
    INJECT(order_service_impl(
        model::repository_hub* repos, 
        auth_service* auth_svc, fruit::Provider<item_service_impl> item_svc));
    ~order_service_impl() override = default;
    void place_order(auth_token token, std::string_view item_id) override;
    void get_my_bought_orders(auth_token token, std::vector<model::order>& dest) override;
    void get_my_sold_orders(auth_token token, std::vector<model::order>& dest) override;
    void get_all_orders(auth_token token, std::vector<model::order>& dest) override;

private:
    model::repository_hub* _repos;
    model::order_repository* _o_repo;
    auth_service_impl* _auth_svc;
    fruit::Provider<item_service_impl> _item_svc;
};
}
}  // namespace hakurei::core