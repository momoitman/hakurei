#pragma once

#include "model/item.h"
#include "model/order.h"
#include "model/user.h"
#include "model/serialization.h"
#include "persistence/abstract_persistence.h"
#include "util/setting.h"

#include <tuple>
#include <memory>

namespace hakurei::core
{
namespace model
{
template<class T>
class repository
{
public:
    repository()
        : _persistence(persistence::get_active_persistence_registry().construct(
            get_active_setting()["persistence"]["persistence_provider"].value_or(""),
                persistence_initializer<T>::name(),
                persistence_initializer<T>::table_desc()
        )) {}

private:
    std::unique_ptr<persistence::abstract_persistence> _persistence;
};

using repository_hub = 
    std::tuple< 
        repository<user>,
        repository<item>,
        repository<order>
    >;
}
}  // namespace hakurei::core