#pragma once

#include "setting.h"
#include "internal_exceptions.h"

#include <string>
#include <stdexcept>
#include <string_view>
#include <fruit/fruit.h>

namespace hakurei::core
{
namespace util
{
class password_hasher
{
public:
    virtual ~password_hasher() = default;
    virtual std::string hash_password(std::string_view password) = 0;
    virtual bool verity_password(std::string_view password, std::string_view hash) = 0;
};

using password_hasher_component = fruit::Component<fruit::Required<setting_t>, password_hasher>;
password_hasher_component get_password_hasher_component();

}
}
