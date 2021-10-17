#include "item_service_intl.h"
#include "item_service_intl2.h"
#include "service/service_exceptions.h"
#include "service/id_generator.h"
#include "util/string_sanitizer.h"

#include <fmt/core.h>

namespace hakurei::core
{
namespace service
{
namespace
{
void verify_price(int price_cents)
{
    if (price_cents <= 0)
        throw invalid_argument_error("You can't have price that's <= 0!");
}
}

item_service_impl::item_service_impl(model::repository_hub* repos, auth_service_intl* auth_svc)
    : _repos(repos), _i_repo(&repos->item_repo()), _auth_svc(auth_svc) {}

std::string item_service_impl::add_item(auth_token token, std::string_view name, int price_cents, std::string_view description)
{
    util::verify_string(name, true);
    util::verify_string(description, false);
    auto u = _auth_svc->get_user_info_ref(token);
    model::item i(
        generate_next_id(_i_repo->get_last_id().value_or("M00000")),
        std::string(name),
        price_cents,
        std::string(description),
        u->id(),
        std::chrono::system_clock::now(),
        model::item_status::on_stock);
    _i_repo->append(i);
    return i.id();
}

void item_service_impl::remove_item(auth_token token, std::string_view id)
{
    auto u = _auth_svc->get_user_info_ref(token);
    auto item = get_item_force(id);
    if (item.seller_uid() != u->id())
        throw access_denied_error("You can only remove item of yourself!");
    item.set_status(model::item_status::deleted);
    _i_repo->save(item);
}

void item_service_impl::set_item(auth_token token, std::string_view id,
                                 std::optional<std::string_view> name, std::optional<int> price_cents,
                                 std::optional<std::string_view> description)
{
    auto u = _auth_svc->get_user_info_ref(token);
    auto item = get_item_force(id);
    if (item.seller_uid() != u->id())
        throw access_denied_error("You can only edit item of yourself!");
    if (name)
    {
        util::verify_string(name.value(), true);
        item.set_name(std::string(name.value()));
    }
    if (price_cents)
    {
        verify_price(price_cents.value());
        item.set_price_cents(price_cents.value());
    }
    if (description)
    {
        util::verify_string(description.value(), false);
        item.set_description(std::string(description.value()));
    }
    _i_repo->save(item);
}

std::optional<model::item> item_service_impl::get_item(std::string_view id)
{
    model::item i;
    if (!_i_repo->load(std::string(id), i))
        return std::nullopt;
    return i;
}

model::item item_service_impl::get_item_force(std::string_view id)
{
    model::item i;
    if (!_i_repo->load(std::string(id), i))
        throw not_found_error(fmt::format("Item with id {} not found", id));
    return i;
}

void item_service_impl::search_item(auth_token token, std::string_view keywords, std::vector<model::item>& dest)
{
    // POTENTIAl SECURITY RISK: leak deleted item if a fake vector is passed in!
    _i_repo->search(std::string(keywords), dest);
}

void item_service_impl::get_all_items(auth_token token, std::vector<model::item>& dest)
{
    _auth_svc->verify_admin_user(token);
    _i_repo->get_all(dest);
}

void item_service_impl::get_my_items(auth_token token, std::vector<model::item>& dest)
{
    auto name = _auth_svc->get_user_info_ref(token)->id();
    _i_repo->find_by_column(4, name, dest);
}

void item_service_impl::mark_item_purchased(std::string_view id)
{
    auto item = get_item_force(id);
    // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
    // ReSharper disable once CppIncompleteSwitchStatement
    switch (item.status())  // NOLINT(clang-diagnostic-switch)
    {
    case model::item_status::sold:
        throw bad_status_error("The item is already sold!");
    case model::item_status::deleted:
        throw bad_status_error("The item was deleted and can't be purchased.");
    }

    item.set_status(model::item_status::sold);
    _i_repo->save(item);
}

item_service_impl2::item_service_impl2(model::repository_hub* repos)
    : _repos(repos), _i_repo(&repos->item_repo()) {}

void item_service_impl2::remove_item_of_user(std::string_view uid)
{
    std::vector<model::item> temp;
    _i_repo->find_by_column(4, std::string(uid), temp);
    for (auto& item : temp)
    {
        if (item.status() == model::item_status::on_stock)
            item.set_status(model::item_status::deleted);
        _i_repo->save(item);  // optimize here?
    }
}
}
}  // namespace hakurei::core