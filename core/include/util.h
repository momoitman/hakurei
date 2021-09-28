#pragma once

#include <system_error>
#include <sstream>
#include <string_view>

namespace hakurei::core
{
inline std::string to_string(std::system_error const& err, std::string_view header)
{
    std::stringstream ss;
    ss << "File I/O error:" << err.what() << "; Code " << err.code();
    return ss.str();
}

}  // namespace hakurei::core