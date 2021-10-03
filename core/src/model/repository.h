#pragma once

#include "config.h"
#include "model/item.h"
#include "model/order.h"
#include "model/user.h"
#include "model/serialization.h"
#include "persistence/abstract_persistence.h"

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
        : _persistence() {}

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