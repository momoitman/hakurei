#include "auth_service_intl.h"

#include "model/repository.h"
#include "util/setting.h"
#include "service/service_exceptions.h"
#include "service/id_generator.h"
#include "util/string_sanitizer.h"

#include <random>
#include <limits>

#include "item_service_intl.h"
#include "order_service_intl.h"

namespace hakurei::core
{
namespace service
{
const std::string _admin_user_name = "admin";

auth_service_impl::auth_service_impl(
    setting_t* setting,
    model::repository_hub* repos, util::password_hasher* hasher,
    item_service_intl2* i_serv)
    : _setting(setting), _item_svc(i_serv),
      _repos(repos), _u_repo(&repos->user_repo()), _password_hasher(hasher),
      _admin_user_name((*setting)["auth"]["admin_user_name"].value_or("admin"))
{}

auth_token generate_auth_token();
void verify_username(std::string_view name, model::user_repository* repo);

auth_token auth_service_impl::register_user(
    std::string_view name,
    std::string_view password,
    std::string_view contact,
    std::string_view address)
{
    verify_username(name, _u_repo);
    util::verify_password(password);
    util::verify_string(contact, true);
    util::verify_string(address, false);
    
    model::user u(
        generate_next_id(_u_repo->get_last_id().value_or("U00000")),
        std::string(name),
        _password_hasher->hash_password(password),
        std::string(contact),
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
        throw invalid_credential_error();
    if (!u.password_obfus())
        throw invalid_credential_error();

    if (!_password_hasher->verity_password(std::string(password), u.password_obfus().value()))
        throw invalid_credential_error();
    return allocate_auth_token(u);
}

void auth_service_impl::logout_user(auth_token token)
{
    _sessions.erase(token);
}

bool auth_service_impl::is_user_admin(auth_token token)
{
    auto ref = get_user_info_ref(token);
    return ref->name() == _admin_user_name;
}

void auth_service_impl::get_all_users(auth_token token, std::vector<model::user>& dest)
{
    verify_admin_user(token);
    _u_repo->get_all(dest);
}

void auth_service_impl::remove_user(auth_token token, std::string_view id)
{
    verify_admin_user(token);
    _item_svc->remove_item_of_user(id);
    _u_repo->remove(std::string(id));
}

model::user auth_service_impl::get_user_info(auth_token token)
{
    return get_user_info_ref(token)->safe_copy();
}

std::optional<std::string> auth_service_impl::get_user_name(std::string_view id)
{
    model::user u;
    if (!_u_repo->find_one_by_column(0, std::string(id), u))
        return std::nullopt;
    return u.name();
}

std::string auth_service_impl::get_user_name(auth_token token)
{
    return get_user_info_ref(token)->name();
}

std::optional<model::user> auth_service_impl::get_user_info(std::string_view id)
{
    model::user u;
    // TODO: security risk: leaking privacy
    if (!_u_repo->find_one_by_column(0, std::string(id), u))
        return std::nullopt;
    return u;
}

void auth_service_impl::set_user_info(
    auth_token token, std::optional<std::string_view> name, std::optional<std::string_view> password,
    std::optional<std::string_view> contact, std::optional<std::string_view> address)
{
    auto u = get_user_info_ref(token);
    if (name && u->name() != name.value())
    {
        if (is_user_admin(token))
            throw access_denied_error("You can't change admin user's name!");
        verify_username(name.value(), _u_repo);
        u->set_name(std::string(name.value()));
    }
    if (password)
    {
        util::verify_password(password.value());
        u->set_password_obfus(_password_hasher->hash_password(password.value()));
    }
    if (contact)
    {
        util::verify_string(contact.value(), true);
        u->set_contact(std::string(contact.value()));
    }
    if (address)
    {
        util::verify_string(address.value(), true);
        u->set_address(std::string(address.value()));
    }

    _u_repo->save(*u);
}

int auth_service_impl::get_user_balance_cents(auth_token token)
{
    return get_user_info_ref(token)->balance_cents();
}

int auth_service_impl::deposit_money(auth_token token, int money_cents)
{
    if (money_cents <= 0)
        throw invalid_argument_error("You can't deposit 0 or negative money.");
    auto u = get_user_info_ref(token);
    u->set_balance_cents(u->balance_cents() + money_cents);
    _u_repo->save(*u);

    return u->balance_cents();
}

void auth_service_impl::deposit_money(std::string_view uid, int money_cents)
{
    if (money_cents <= 0)
        throw invalid_argument_error("You can't deposit 0 or negative money.");
    auto u = get_user_info(uid);
    u->set_balance_cents(u->balance_cents() + money_cents);
    _u_repo->save(*u);
}

model::user* auth_service_impl::get_user_info_ref(auth_token token)
{
    auto iter = _sessions.find(token);
    if (iter == _sessions.end())
        throw invalid_token_error();
    return &iter->second;
}

void auth_service_impl::withdraw_money(auth_token token, int money_cents)
{
    auto u = get_user_info_ref(token);
    if (u->balance_cents() < money_cents)
        throw insufficient_balance_error(u->balance_cents(), money_cents);
    u->set_balance_cents(u->balance_cents() - money_cents);
    _u_repo->save(*u);
}

void auth_service_impl::verify_admin_user(auth_token token)
{
    if (!is_user_admin(token))
        throw access_denied_error("This operation is only available for admin user");
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
    while (_sessions.find(t) != _sessions.end());
    _sessions.emplace(t, u);
    return t;
}

void verify_username(std::string_view name, model::user_repository* repo)
{
    util::verify_string(name, true);
    model::user u;
    auto existing = repo->find_one_by_column(1, std::string(name), u);
    if (existing)
        throw duplication_error("An user with the the specified user name already exists!");
}

}  // namespace service
}  // namespace hakurei::core