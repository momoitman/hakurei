#include "auth_service_intl.h"

#include "model/repository.h"
#include "service/service_exceptions.h"

#include <random>
#include <limits>

namespace hakurei::core
{
namespace service
{
auth_service_impl::auth_service_impl(model::repository_hub* repos, util::password_hasher* hasher)
    : _repos(repos), _u_repo(&repos->user_repo()), _password_hasher(hasher)
{}

std::string user_next_id(std::string const& previous);
auth_token generate_auth_token();

auth_token auth_service_impl::register_user(
    std::string_view name,
    std::string_view password,
    std::string_view contact,
    std::string_view address)
{
    model::user u;
    auto existing = _u_repo->find_one_by_column(1, std::string(name), u);
    if (existing)
        throw duplication_error("User name exists!");

    u = model::user(
        user_next_id(_u_repo->get_last_id().value_or("U00000")),
        std::string(name),
        _password_hasher->hash_password(password),
        std::string(address),
        0);
    _u_repo->append(u);

    return allocate_auth_token(u);
}

auth_token auth_service_impl::login_user(std::string_view username, std::string_view password)
{
    model::user u;
    auto existing = _u_repo->find_one_by_column(1, std::string(username), u);
    if (!existing)
        throw invalid_credential_error("Invalid username or password!");
    if (!u.password_obfus())
        throw invalid_credential_error("Invalid user state!");

    if (!_password_hasher->verity_password(std::string(password), u.password_obfus().value()))
        throw invalid_credential_error("Invalid username or password!");
    return allocate_auth_token(u);
}

void auth_service_impl::logout_user(auth_token token)
{
    _sessions.erase(token);
}

model::user auth_service_impl::get_user_info(auth_token token)
{
    auto iter = _sessions.find(token);
    return iter->second.safe_copy();
}

std::optional<model::user> auth_service_impl::get_user_info(std::string_view id)
{
    model::user u;
    if (!_u_repo->find_one_by_column(1, std::string(id), u))
        return std::nullopt;
    return u;
}

void auth_service_impl::set_user_info(
    auth_token token, std::optional<std::string_view> name, std::optional<std::string_view> password,
    std::optional<std::string_view> contact, std::optional<std::string_view> address)
{
    auto u = get_user_info_ref(token);
    if (name)
    {
        model::user u2;
        auto existing = _u_repo->find_one_by_column(1, std::string(name.value()), u2);
        if (existing)
            throw duplication_error("User name exists!");
        u->set_name(std::string(name.value()));
    }
    if (password)
        u->set_password_obfus(_password_hasher->hash_password(password.value()));
    if (contact)
        u->set_contact(std::string(contact.value()));
    if (address)
        u->set_address(std::string(address.value()));

    _u_repo->save(*u);
}

int auth_service_impl::get_user_balance_cents(auth_token token)
{
    return get_user_info_ref(token)->balance_cents();
}

int auth_service_impl::deposit_money(auth_token token, int money_cents)
{
    if (money_cents <= 0)
        throw std::invalid_argument("money < 0");
    auto u = get_user_info_ref(token);
    u->set_balance_cents(u->balance_cents() + money_cents);
    _u_repo->save(*u);

    return u->balance_cents();
}

model::user* auth_service_impl::get_user_info_ref(auth_token token)
{
    auto iter = _sessions.find(token);
    if (iter == _sessions.end())
        throw invalid_token_error(std::to_string(token));
    return &iter->second;
}

auth_token generate_auth_token()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution ud(
        std::numeric_limits<auth_token>::min(),
        std::numeric_limits<auth_token>::max());
    return ud(gen);
}

auth_token auth_service_impl::allocate_auth_token(model::user const& u)
{
    auth_token t;
    do 
        t = generate_auth_token();
    while (_sessions.find(t) == _sessions.end());
    _sessions.emplace(t, u);
    return t;
}

}  // namespace service
}  // namespace hakurei::core