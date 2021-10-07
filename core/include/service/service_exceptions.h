#pragma once

#include <stdexcept>

namespace hakurei::core
{
namespace service
{
class duplication_error : public std::runtime_error
{
public:
    explicit duplication_error(const std::string& basic_string) : runtime_error(basic_string) { }
    explicit duplication_error(const char* string) : runtime_error(string) { }
    explicit duplication_error(const runtime_error& rt) : runtime_error(rt) { }
};

class invalid_token_error : public std::runtime_error
{
public:
    explicit invalid_token_error(const std::string& basic_string) : runtime_error(basic_string) {}
    explicit invalid_token_error(const char* string) : runtime_error(string) {}
    explicit invalid_token_error(const runtime_error& rt) : runtime_error(rt) {}
};

class invalid_credential_error : public std::runtime_error
{
public:
    explicit invalid_credential_error(const std::string& basic_string) : runtime_error(basic_string) {}
    explicit invalid_credential_error(const char* string) : runtime_error(string) {}
    explicit invalid_credential_error(const runtime_error& rt) : runtime_error(rt) {}
};
}
}  // namespace hakurei::core