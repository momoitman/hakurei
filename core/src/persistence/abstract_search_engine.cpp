#include "abstract_search_engine.h"

namespace hakurei::core
{
namespace persistence
{
search_engine_factory_registry& get_search_engine_registry()
{
    static search_engine_factory_registry default_search_factory("search_engine");
    return default_search_factory;
}

}
}  // namespace hakurei::core