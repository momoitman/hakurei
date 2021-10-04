#include "repository.h"

namespace hakurei::core
{
namespace model
{
std::shared_ptr<repository_hub> default_repository_hub;

std::shared_ptr<repository_hub> get_active_repositories()
{
    if (!default_repository_hub)
        default_repository_hub = std::make_shared<repository_hub>();

    return default_repository_hub;
}
}
}  // namespace hakurei::core