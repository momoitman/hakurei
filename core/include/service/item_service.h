#pragma once

#include "model/item.h"
#include "service/auth_service.h"

#include <string>
#include <string_view>
#include <optional>
#include <vector>

namespace hakurei::core
{
namespace service
{
class item_service
{
public:
    virtual std::string add_item(auth_token token, std::string_view name, int price_cents, std::string_view descrption) = 0;
    virtual void set_item(auth_token token, std::string_view id,
                          std::optional<std::string_view> name, std::optional<int> price_cents,
                          std::optional<std::string_view> descrption) = 0;
    virtual void remove_item(auth_token token, std::string_view id) = 0;
    virtual std::optional<model::item> get_item(std::string_view id) = 0;
    virtual std::vector<model::item> search_item(std::string_view keywords, bool on_stock_only) = 0;
};

}
}  // namespace hakurei::core