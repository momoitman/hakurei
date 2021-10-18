#pragma once

#include <string_view>

namespace hakurei::core
{
namespace util
{

void verify_string(std::string_view str, bool strong);
void verify_password(std::string_view password);

}
}  // namespace hakurei::core