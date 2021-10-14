#include "item_service_intl.h"
#include "order_service_intl.h"
#include "id_generator.h"
#include "service/service_exceptions.h"

namespace hakurei::core
{
namespace service
{

order_service_impl::order_service_impl(model::repository_hub* repos, 
    auth_service* auth_svc, fruit::Provider<item_service_impl> item_svc)
    : _repos(repos), _o_repo(&repos->order_repo()),
    _auth_svc(reinterpret_cast<auth_service_impl*>(auth_svc)), _item_svc(item_svc) {}

void order_service_impl::place_order(auth_token token, std::string_view item_id)
{
    auto item = _item_svc.get()->get_item_force(item_id);
    if (item.status() != model::item_status::on_stock)
        throw bad_status_error("The item isn't on stock!");
    _auth_svc->withdraw_money(token, item.price_cents());
    _item_svc.get()->mark_item_purchased(item_id);
    _o_repo->append(
        model::order(
            generate_next_id(_o_repo->get_last_id().value_or("T00000")),
            std::string(item_id),
            item.price_cents(),
            std::chrono::system_clock::now(),
            item.seller_uid(),
            _auth_svc->get_user_info_ref(token)->id()
        )
    );
}

void order_service_impl::get_my_bought_orders(auth_token token, std::vector<model::order>& dest)
{
    _o_repo->find_by_column(5, _auth_svc->get_user_info_ref(token)->id(), dest);
}

void order_service_impl::get_my_sold_orders(auth_token token, std::vector<model::order>& dest)
{
    _o_repo->find_by_column(4, _auth_svc->get_user_info_ref(token)->id(), dest);
}

void order_service_impl::get_all_orders(auth_token token, std::vector<model::order>& dest)
{
    _auth_svc->verify_admin_user(token);
    _o_repo->get_all(dest);
}
}
}  // namespace hakurei::core