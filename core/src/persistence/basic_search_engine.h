#pragma once

#include "abstract_search_engine.h"

namespace hakurei::core
{
namespace persistence
{

class basic_search_engine : public abstract_search_engine
{
public:
    basic_search_engine() = default;
    ~basic_search_engine() = default;
    void attach_persistence(abstract_persistence*, std::vector<int> columns) override;
    void build_index() override { } // No need to index
    void search(std::string_view keywords, std::vector<table::row_t>& dest) override;

    void notify_change(table::row_t const& row) override { } // No need to index
    void notify_delete(table::cell_t const& id) override { } // No need to index
    void notify_append(table::row_t const& row) override { } // No need to index

private:
    abstract_persistence* _persistence = nullptr;
    std::vector<int> _columns = {};

    std::vector<int> _kmp_lpz_buf;
};

bool register_basic_search_engine();

}
}  // namespace hakurei::core