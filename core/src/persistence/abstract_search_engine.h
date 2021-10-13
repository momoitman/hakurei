#pragma once

#include "util/factory_registry.h"
#include "util/setting.h"
#include "table.h"

#include <functional>

namespace hakurei::core
{
namespace persistence
{

class abstract_persistence;

class abstract_search_engine
{
public:
    virtual ~abstract_search_engine() = default;
    virtual void attach_persistence(abstract_persistence*, std::vector<int> columns) = 0;
    virtual void build_index() = 0;
    virtual void search(std::string_view keywords, std::vector<table::row_t>& dest) = 0;

    virtual void notify_change(table::row_t const& row) = 0;
    virtual void notify_delete(table::cell_t const& id) = 0;
    virtual void notify_append(table::row_t const& row) = 0;
};

using search_engine_factory_registry = factory_registry<
    std::unique_ptr<abstract_search_engine>,
    std::unique_ptr<abstract_search_engine> (*)(setting_t const&),
    setting_t const&>;
search_engine_factory_registry& get_search_engine_registry();

using search_engine_factory = std::function<std::unique_ptr<abstract_search_engine>()>;

}
}  // namespace hakurei::core