#pragma once

#include <string>
#include <string_view>

namespace hakurei::core
{
namespace service
{
std::string generate_next_id(std::string_view previous);
}
}  // namespace hakurei::core