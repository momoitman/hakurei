#pragma once
#include "service/item_service.h"

#pragma once

#include "model/item.h"
#include "model/repository.h"
#include "auth_service_intl.h"

#include <string>
#include <string_view>
#include <fruit/fruit.h>

namespace hakurei::core
{
namespace service
{
class item_service_impl : public item_service
{
public:
    INJECT(item_service_impl(model::repository_hub* repos, auth_service* auth_svc));
    std::string add_item(auth_token token, std::string_view name, int price_cents, std::string_view descrption) override;
    void remove_item(auth_token token, std::string_view id) override;
    void set_item(auth_token token, std::string_view id,
                  std::optional<std::string_view> name, std::optional<int> price_cents,
                  std::optional<std::string_view> descrption) override;

    std::optional<model::item> get_item(std::string_view id) override;
    model::item get_item_force(std::string_view id);
    std::vector<model::item> search_item(std::string_view keywords, bool on_stock_only) override;

    void mark_item_purchased(std::string_view id);

private:
    model::repository_hub* _repos;
    model::item_repository* _i_repo;
    auth_service_impl* _auth_svc;
};

}  // namespace service
}  // namespace hakurei::core