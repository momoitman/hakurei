#define TOML_IMPLEMENTATION
#include "setting.h"

#include <sstream>

#include <spdlog/spdlog.h>

using namespace std::literals;

namespace hakurei::core
{
//module_setting get_module_setting()
setting_t* create_setting()
{
    setting_t* setting;
    try
    {
        setting = new setting_t(toml::parse_file("settings.toml"sv));
    }
    catch (const toml::parse_error& err)
    {
        std::ostringstream temp;
        temp << err;
        spdlog::critical("[settings] Failed to parse settings.toml, using default: {}", temp.str());
        setting = new setting_t;
    }

    return setting;
}

setting_component get_setting_component()
{
    return fruit::createComponent()
        .registerProvider([]() { return create_setting(); });
    
}
}  // namespace hakurei::core