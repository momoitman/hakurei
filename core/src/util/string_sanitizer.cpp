#include "util/string_sanitizer.h"

#include "service/service_exceptions.h"

#include <string>
using namespace std::literals;

namespace hakurei::core
{
namespace util
{
const std::string_view illegal_chars = "\0"sv;
const std::string_view illegal_chars_strong = "\0\n\\/:*?\""sv;
const std::string_view illegal_chars_password = "\0\n"sv;

void verify_string(std::string_view str, bool strong)
{
    if (strong && str.empty())
        throw service::illegal_string_error("String can't be empty!");
    auto ic = strong ? illegal_chars_strong : illegal_chars;
    for (char ch : str)
        if (ic.find(ch) != std::string_view::npos)
            throw service::illegal_string_error("illegal character in string!");
}

void verify_password(std::string_view password)
{
    if (password.size() < 6)
        throw service::illegal_string_error("Password too weak!");
    // TODO check for UpperLowerNumber?
    for (char ch : password)
        if (illegal_chars_password.find(ch) != std::string_view::npos)
            throw service::illegal_string_error("illegal character in password!");
}

}  // namespace util
}  // namespace hakurei::core