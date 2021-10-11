#pragma once

#include "model/user.h"

#include <cstdint>
#include <memory>
#include <string_view>
#include <vector>

namespace hakurei::core
{
namespace service
{
using auth_token = std::uint64_t;

class auth_service
{
public:
    virtual ~auth_service() = default;
    virtual auth_token register_user(std::string_view name, std::string_view password,
                                     std::string_view contact, std::string_view address) = 0;
    virtual auth_token login_user(std::string_view username, std::string_view password) = 0;
    virtual void logout_user(auth_token token) = 0;

    virtual bool is_user_admin(auth_token token) = 0;
    virtual model::user get_user_info(auth_token token) = 0;
    virtual void get_all_users(auth_token token, std::vector<model::user>& dest) = 0;
    virtual void remove_user(auth_token token, std::string_view id) = 0;
    virtual std::optional<std::string> get_user_name(std::string_view id) = 0;
    virtual std::optional<model::user> get_user_info(std::string_view id) = 0;
    virtual void set_user_info(auth_token token, std::optional<std::string_view> name, std::optional<std::string_view> password,
                               std::optional<std::string_view> contact, std::optional<std::string_view> address) = 0;
    virtual int get_user_balance_cents(auth_token token) = 0;

    virtual int deposit_money(auth_token token, int money_cents) = 0;
};
}  // namespace service
}  // namespace hakurei::core