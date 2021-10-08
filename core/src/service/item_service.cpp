#include "item_service_intl.h"
#include "service/service_exceptions.h"
#include "util/string_sanitizer.h"

#include <fmt/core.h>

namespace hakurei::core
{
namespace service
{
// TODO: impl
std::string item_next_id(std::string const& previous);

item_service_impl::item_service_impl(model::repository_hub* repos, auth_service* auth_svc)
    : _repos(repos), _i_repo(&repos->item_repo()), _auth_svc(reinterpret_cast<auth_service_impl*>(auth_svc)) {}

std::string item_service_impl::add_item(auth_token token, std::string_view name, int price_cents, std::string_view description)
{
    util::verify_string(name, true);
    util::verify_string(description, false);
    auto u = _auth_svc->get_user_info_ref(token);
    model::item i(
        item_next_id(_i_repo->get_last_id().value_or("M00001")),
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
                                 std::optional<std::string_view> descrption)
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
        item.set_price_cents(price_cents.value());
    if (descrption)
    {
        util::verify_string(descrption.value(), false);
        item.set_description(std::string(descrption.value()));
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
        throw not_found_error(fmt::format("Item id={} not found", id));
    return i;
}

std::vector<model::item> item_service_impl::search_item(std::string_view keywords, bool on_stock_only)
{
    // TODO impl
    std::vector<model::item> items;
    return items;
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
}
}  // namespace hakurei::core