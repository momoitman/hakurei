#include "csv_persistence.h"

#include "util.h"

#include <filesystem>
#include <fstream>
#include <algorithm>

#include <csv.hpp>

namespace hakurei::core
{
namespace persistence
{
const std::string FILE_EXTENSION = ".csv";

csv_persistence::csv_persistence(std::string const& name, table::table_desc const& table_spec, std::string base_path)
    : abstract_memory_persistence(name, table_spec),
      _path(std::filesystem::path(base_path) / (name + FILE_EXTENSION))
{
    std::error_code ec;
    create_directories(std::filesystem::path(base_path), ec);
    if (ec)
    {
        // TODO: log warn
    }
}

void csv_persistence::save()
{
    std::ofstream stream;
    stream.exceptions(std::ofstream::badbit | std::ofstream::failbit);
    try
    {
        stream.open(_path, std::ios_base::out | std::ios_base::trunc);
        auto writer = csv::make_csv_writer(stream);
        writer << _table_desc.column_names();
        for (auto& [_, row] : _table->rows())
        {
            // TODO: Datetime notation
            std::vector<std::string> row_string;
            row_string.reserve(row.size());
            std::transform(
                row.begin(), row.end(),
                std::back_inserter(row_string),
                [](table::cell_t const& it) -> auto
                {
                    return table::to_string(it);
                });
            writer << row_string;
        }
        stream.flush();
    }
    catch (std::system_error& err)
    {
        throw persistence_error(to_string(err, "File I/O Error: "));
    }
}

void csv_persistence::load()
{
    if (!std::filesystem::exists(_path))
        return;

    auto& rows = _table->rows();
    rows.clear();
    auto const& desc = _table->desc();
    auto const& types = desc.column_types();
    try
    {
        auto reader = csv::CSVReader(_path, csv::CSVFormat().header_row(0));
        for (auto& csv_row : reader)
        {
            auto size = types.size();
            if (csv_row.size() < size)
                continue; // TODO: log warn
            table::row_t row;
            row.reserve(csv_row.size());
            try
            {
                for (size_t i = 0; i < size; i++)
                {
                    switch (types[size])
                    {
                    case table::table_column_type::int32:
                        row[i] = csv_row[i].get<int32_t>();
                        break;
                    // TODO: Datetime notation
                    case table::table_column_type::datetime:
                    case table::table_column_type::int64:
                        row[i] = csv_row[i].get<int64_t>();
                        break;
                    case table::table_column_type::decimal:
                        row[i] = csv_row[i].get<double>();
                        break;
                    case table::table_column_type::string:
                        row[i] = csv_row[i].get<std::string>();
                        break;
                    default:
                        // TODO: log warn
                        continue;
                    }
                }
            } catch (std::runtime_error& err)
            {
                // TODO: log warn
                continue;
            }
            auto header = row[0];
            rows.insert(std::make_pair(header, std::move(row)));
        }
    }
    catch (std::system_error& err)
    {
        throw persistence_error(to_string(err, "File I/O Error: "));
    }
}

}  // namespace persistence
}  // namespace hakurei::core