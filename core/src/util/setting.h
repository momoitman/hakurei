#pragma once
#define TOML_HEADER_ONLY 0
#include <toml.hpp>

namespace hakurei::core
{
toml::table& get_active_setting();
}  // namespace hakurei::core