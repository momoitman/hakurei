#include "basic_search_engine.h"

#include "abstract_persistence.h"

namespace hakurei::core
{
namespace persistence
{

namespace
{
// stolen from https://www.codespeedy.com/knuth-morris-pratt-kmp-algorithm-in-c/
void kmp_lps(std::string_view txt, std::vector<int>& lps)
{
    lps[0] = 0;
    size_t len = 0;
    size_t i = 1;
    while (i < txt.length())
    {
        if (txt[i] == txt[len])
        {
            len++;
            lps[i] = len;
            i++;
            continue;
        }
        if (len == 0)
        {
            lps[i] = 0;
            i++;
        }
        else
            len = lps[len - 1];
    }
}
bool kmp_search(std::string_view text, std::string_view pattern, std::vector<int>& lps_buf)
{
    auto tLen = text.length();
    auto pLen = pattern.length();
    lps_buf.resize(pLen);

    kmp_lps(pattern, lps_buf);  // This function constructs the Lps array.

    size_t i = 0, j = 0;
    while (i < tLen)
    {
        if (pattern[j] == text[i])
        {
            i++;
            j++;
        }  // If there is a match continue.
        if (j == pLen)
            return true;
        if (i < tLen && pattern[j] != text[i])
        {               // If there is a mismatch
            if (j == 0) // if j becomes 0 then simply increment the index i
                i++;
            else
                j = lps_buf[j - 1];  //Update j as Lps of last matched character
        }
    }
    return false;
}
}

// we can't do this in a static-library env: https://www.cppstories.com/2018/02/static-vars-static-lib/
//static bool basic_search_engine_registered = register_basic_search_engine();

void basic_search_engine::attach_persistence(abstract_persistence* persistence, std::vector<int> columns)
{
    _persistence = persistence;
    _columns = columns;
}

void basic_search_engine::search(std::string_view keywords, std::vector<table::row_t>& dest)
{
    dest.clear();
    if (_persistence == nullptr || _columns.empty())
        return;

    auto rows = _persistence->all_rows_table()->rows();
    for (auto& [_, row] : rows)
    {
        for (auto c : _columns)
        {
            auto& cell = row[c];
            if (!std::holds_alternative<std::string>(cell))
                continue;
            auto& str = std::get<std::string>(cell);
            if (kmp_search(str, keywords, _kmp_lpz_buf))
            {
                dest.emplace_back(row);
                break;
            }
        }
    }
}

std::unique_ptr<abstract_search_engine> create_basic_search_engine(setting_t const&)
{
    return std::make_unique<basic_search_engine>();
}

bool register_basic_search_engine()
{
    get_search_engine_registry().register_factory("basic_search_engine", create_basic_search_engine);
    return true;
}
}
}  // namespace hakurei::core