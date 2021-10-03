#define TOML_IMPLEMENTATION
#include "setting.h"

#include <sstream>

#include <spdlog/spdlog.h>

using namespace std::literals;

namespace hakurei::core
{
std::unique_ptr<toml::table> default_setting;
toml::table& get_active_setting()
{
    if (!default_setting)
    {
        try
        {
            default_setting = std::make_unique<toml::table>(toml::parse_file("settings.toml"sv));
        }
        catch (const toml::parse_error& err)
        {
            std::ostringstream temp;
            temp << err;
            spdlog::critical("[settings] Failed to parse settings.toml, using default: {}", temp.str());
            default_setting = std::make_unique<toml::table>();
        }
    }
    return *default_setting;
}

}  // namespace hakurei::core