#pragma once

#include "abstract_persistence.h"
#include "table.h"

#include <string>
#include <memory>

namespace hakurei::core
{
namespace persistence
{
class csv_persistence : public abstract_memory_persistence
{
public:
    using abstract_persistence::save;
    using abstract_persistence::load;
    void save() override;
    void load() override;

    csv_persistence(std::string const& name, table::table_desc const& table_spec,
        std::string const& base_path, std::unique_ptr<abstract_search_engine> search_eng);
    ~csv_persistence() override = default;

    csv_persistence& operator=(const csv_persistence& other) = delete;
    csv_persistence& operator=(csv_persistence&& other) noexcept = delete;

private:
    std::string _path;
};

bool register_csv_persistence();

}  // namespace persistence
}  // namespace hakurei::core