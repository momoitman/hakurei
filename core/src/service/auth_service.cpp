#include "service/auth_service.h"

#include "model/repository.h"

namespace hakurei::core
{
namespace service
{
struct auth_service::impl
{
    auth_token register_user(std::string_view name, std::string_view password,
                             std::string_view contact, std::string_view address);
    auth_token login_user(std::string_view username, std::string_view password);
    void logout_user(auth_token token);

    model::user get_user_info(auth_token token);
    void set_user_info(std::optional<std::string_view> name, std::optional<std::string_view> password,
                       std::optional<std::string_view> contact, std::optional<std::string_view> address);
    int get_user_balance_cents(auth_token token);

    int deposit_money(auth_token token, int money_cents);
};

auth_service::auth_service()
    : _pImpl(std::make_unique<impl>())
{

}
}  // namespace service
}  // namespace hakurei::core