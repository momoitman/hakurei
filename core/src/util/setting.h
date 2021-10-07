#pragma once
#define TOML_HEADER_ONLY 0
#include <toml.hpp>
#include <fruit/fruit.h>

namespace hakurei::core
{
using setting_t = toml::table;
using setting_component = fruit::Component<setting_t>;
setting_component get_setting_component();
}  // namespace hakurei::core