#pragma once

#include <cassert>
#include <string>
#include <cstdint>
#include <vector>
#include <variant>
#include <map>
#include <stdexcept>
#include <iomanip>

#include <fmt/core.h>

namespace hakurei::core
{
namespace persistence
{
namespace table
{
enum class table_column_type : std::uint8_t
{
    int32 = 1,
    int64 = 2,
    decimal = 3,
    string = 4,
    datetime = 5
};

class table_desc
{
public:
    // Default: take column 0 as the primary key!
    [[nodiscard]] std::vector<std::string> const& column_names() const { return _column_names; }
    [[nodiscard]] std::vector<table_column_type> const& column_types() const { return _column_types; }
    [[nodiscard]] size_t size() const { return _column_types.size(); }
    table_desc(
        std::initializer_list<std::string> names,
        std::initializer_list<table_column_type> types
    ): _column_names(names), _column_types(types)
    {
        if (names.size() != types.size())
            throw std::invalid_argument("Length of column names and column types don't match.");
        if (names.size() == 0)
            throw std::invalid_argument("Table must contain at least 1 column.");
    }

private:
    // - Why not map? - Map is not so friendly for index-based manipulations.
    std::vector<std::string> _column_names;
    std::vector<table_column_type> _column_types;
};

using cell_t = std::variant<std::int32_t, std::int64_t, double, std::string>;
using row_t = std::vector<cell_t>;

inline void check_row_valid(row_t const& row)
{
    if (row.empty())
        throw std::invalid_argument("Bad table row: nColumns <= 0");
}

class table
{
public:
    using container_t = std::map<cell_t, row_t>;
    container_t& rows() { return _rows; }
    container_t const& rows() const { return _rows; }
    [[nodiscard]] size_t size() const { return _rows.size(); }
    [[nodiscard]] table_desc const& desc() const { return _desc; }

    table(table_desc desc)
        : _desc(std::move(desc)) {}

    void check_row_fit(row_t const& row)
    {
        check_row_valid(row);
        if (row.size() < _desc.size())
            throw std::invalid_argument(
                fmt::format("Bad row: nColumns {} < table nColumns {}", row.size(), _desc.size())
            );
#ifndef NDEBUG
        for (int i = 0; i < _desc.size(); i++)
        {
            switch (_desc.column_types()[i])
            {
            case table_column_type::int32:
                assert(std::holds_alternative<std::int32_t>(row[i]));
                break;
            case table_column_type::datetime:
            case table_column_type::int64:
                assert(std::holds_alternative<std::int64_t>(row[i]));
                break;
            case table_column_type::decimal:
                assert(std::holds_alternative<double>(row[i]));
                break;
            case table_column_type::string:
                assert(std::holds_alternative<std::string>(row[i]));
                break;
            default:
                assert(false && "Unknown table column type!");
            }
        }
#endif
    }

private:
    container_t _rows;
    table_desc _desc;
};

struct cell_to_string_s
{
    int precision;
    cell_to_string_s(int precision) : precision(precision) { }

    std::string operator()(std::int32_t v) const { return std::to_string(v); }
    std::string operator()(std::int64_t v) const { return std::to_string(v); }
    std::string operator()(std::string const& v) const { return v; }
    std::string operator()(double v) const { return fmt::format("{:.{}f}", v, precision); }
};
inline std::string to_string(cell_t const& row, int precision = 2)
{
    // TODO optimize: to_string_to to avoid memory allocation
    return std::visit(cell_to_string_s(precision), row);
}

}
}  // namespace persistence
}  // namespace hakurei::core