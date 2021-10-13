#include "repository.h"

namespace hakurei::core
{
namespace model
{
repository_component get_repository_component()
{
    return fruit::createComponent()
        .install(persistence::get_persistence_component);  // repository_hub is automatically bound
}
}
}  // namespace hakurei::core