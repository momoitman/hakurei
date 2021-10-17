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

class item_service_intl : public item_service
{
public:
    virtual void mark_item_purchased(std::string_view id) = 0;
};

class item_service_impl : public item_service_intl
{
public:
    INJECT(item_service_impl(model::repository_hub* repos, auth_service_intl* auth_svc));
    ~item_service_impl() override = default;
    std::string add_item(auth_token token, std::string_view name, int price_cents, std::string_view description) override;
    void remove_item(auth_token token, std::string_view id) override;
    void set_item(auth_token token, std::string_view id,
                  std::optional<std::string_view> name, std::optional<int> price_cents,
                  std::optional<std::string_view> description) override;

    std::optional<model::item> get_item(std::string_view id) override;
    model::item get_item_force(std::string_view id);
    void search_item(auth_token token, std::string_view keywords, std::vector<model::item>& dest) override;
    void get_all_items(auth_token token, std::vector<model::item>& dest) override;
    void get_my_items(auth_token token, std::vector<model::item>& dest) override;

    void mark_item_purchased(std::string_view id) override;

private:
    model::repository_hub* _repos;
    model::item_repository* _i_repo;
    auth_service_intl* _auth_svc;
};

}  // namespace service
}  // namespace hakurei::core