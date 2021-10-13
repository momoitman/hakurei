#pragma once

#include "model/repository.h"

#include <string_view>

namespace hakurei::core
{
namespace service
{
// VERY DIRTY APPROACH HERE
class item_service_intl2
{
public:
    virtual void remove_item_of_user(std::string_view uid) = 0;
};

class item_service_impl2 : public item_service_intl2
{
public:
    INJECT(item_service_impl2(model::repository_hub* repos));
    ~item_service_impl2() = default;
    void remove_item_of_user(std::string_view uid) override;

private:
    model::repository_hub* _repos;
    model::item_repository* _i_repo;
};
}
}  // namespace hakurei::core