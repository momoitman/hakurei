#include "csv_persistence.h"

#include "util/setting.h"

#include <filesystem>
#include <fstream>
#include <algorithm>
#include <string>
#include <string_view>

#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <csv.hpp>

namespace hakurei::core
{
namespace persistence
{
const std::string FILE_EXTENSION = ".csv";
bool register_csv_persistence();
bool _csv_persistence_registered = register_csv_persistence();

csv_persistence::csv_persistence(
    std::string const& name, table::table_desc const& table_spec, std::string const& base_path, 
    std::unique_ptr<abstract_search_engine> search_eng)
    : abstract_memory_persistence(name, table_spec, std::move(search_eng)),
      _path(std::filesystem::path(base_path) / (name + FILE_EXTENSION))
{
    std::error_code ec;
    create_directories(std::filesystem::path(base_path), ec);
    if (ec && ec != std::errc::no_such_file_or_directory)
        spdlog::warn("[pers] Failed to create directory {}: {}; Code: {}", 
            base_path, ec.message(), ec.value());
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
        throw persistence_error(fmt::format("File I/O Error: {}; Code: {}", err.what(), err.code().value()));
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
            {
                spdlog::warn("[pers] Malformed CSV row (in JSON):{}, size mismatch: {}",
                    csv_row.to_json(), csv_row.size());
                continue;
            }
            table::row_t row;
            row.reserve(size);
            try
            {
                for (size_t i = 0; i < size; i++)
                {
                    switch (types[i])
                    {
                    case table::table_column_type::int32:
                        row.push_back(csv_row[i].get<int32_t>());
                        break;
                    // TODO: Datetime notation
                    case table::table_column_type::datetime:
                    case table::table_column_type::int64:
                        row.push_back(csv_row[i].get<int64_t>());
                        break;
                    case table::table_column_type::decimal:
                        row.push_back(csv_row[i].get<double>());
                        break;
                    case table::table_column_type::string:
                        row.emplace_back(std::string(csv_row[i].get_sv()));
                        break;
                    default:
                        assert(false && "table_column_type didn't exhaust!");
                    }
                }
            } catch (std::runtime_error& err)
            {
                spdlog::warn("[pers] Malformed CSV row (in JSON):{}, type mismatch: {}",
                    csv_row.to_json(), err.what());
                continue;
            }
            auto header = row[0];
            rows.insert(std::make_pair(header, std::move(row)));
        }
    }
    catch (std::system_error& err)
    {
        spdlog::warn(
            "[pers] File I/O Error: {}; Code: {}", err.what(), err.code().value());
        //throw persistence_error(fmt::format("File I/O Error: {}; Code: {}", err.what(), err.code().value()));
    }

    _search_eng->build_index();
}

std::unique_ptr<abstract_persistence> create_csv_persistence(
    setting_t const& setting, search_engine_factory search_factory, std::string const& name, table::table_desc const& table_desc)
{
    return std::make_unique<csv_persistence>(
        name, table_desc, 
        setting["persistence"]["csv"]["basic_path"].value_or("./data"),
        search_factory());
}

bool register_csv_persistence()
{
    get_persistence_registry().register_factory("csv_persistence", create_csv_persistence);
    return true;
}

}  // namespace persistence
}  // namespace hakurei::core