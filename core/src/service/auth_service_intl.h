#pragma once
#include "service/auth_service.h"

#include "model/repository.h"
#include "util/password_hasher.h"
#include <fruit/fruit.h>

namespace hakurei::core
{
namespace service
{
class auth_service_impl : public auth_service
{
public:
    INJECT(auth_service_impl(model::repository_hub* repos, util::password_hasher* hasher));
    ~auth_service_impl() override = default;
    auth_token register_user(std::string_view name, std::string_view password, std::string_view contact, std::string_view address) override;
    auth_token login_user(std::string_view username, std::string_view password) override;
    void logout_user(auth_token token) override;
    model::user get_user_info(auth_token token) override;
    std::optional<std::string> get_user_name(std::string_view id) override;
    std::optional<model::user> get_user_info(std::string_view id) override;
    void set_user_info(
        auth_token token, std::optional<std::string_view> name, std::optional<std::string_view> password,
        std::optional<std::string_view> contact, std::optional<std::string_view> address) override;
    int get_user_balance_cents(auth_token token) override;
    int deposit_money(auth_token token, int money_cents) override;

    model::user* get_user_info_ref(auth_token token);

private:
    auth_token allocate_auth_token(model::user const& u);

    model::repository_hub* _repos;
    model::user_repository* _u_repo;
    util::password_hasher* _password_hasher;
    std::unordered_map<auth_token, model::user> _sessions; // IMPORTANT: Remember to update both user in sessions and repository!
};


}  // namespace service
}  // namespace hakurei::core