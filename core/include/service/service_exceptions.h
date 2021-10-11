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

class not_found_error : public std::runtime_error
{
public:
    explicit not_found_error(const std::string& basic_string) : runtime_error(basic_string) {}
    explicit not_found_error(const char* string) : runtime_error(string) {}
    explicit not_found_error(const runtime_error& rt) : runtime_error(rt) {}
};

class bad_status_error : public std::runtime_error
{
public:
    explicit bad_status_error(const std::string& basic_string) : runtime_error(basic_string) {}
    explicit bad_status_error(const char* string) : runtime_error(string) {}
    explicit bad_status_error(const runtime_error& rt) : runtime_error(rt) {}
};

class access_denied_error : public std::runtime_error
{
public:
    explicit access_denied_error(const std::string& basic_string) : runtime_error(basic_string) {}
    explicit access_denied_error(const char* string) : runtime_error(string) {}
    explicit access_denied_error(const runtime_error& rt) : runtime_error(rt) {}
};

class illegal_string_error : public std::runtime_error
{
public:
    explicit illegal_string_error(const std::string& basic_string) : runtime_error(basic_string) {}
    explicit illegal_string_error(const char* string) : runtime_error(string) {}
    explicit illegal_string_error(const runtime_error& rt) : runtime_error(rt) {}
};

class id_gen_failed_error : public std::runtime_error
{
public:
    explicit id_gen_failed_error(const std::string& basic_string) : runtime_error(basic_string) {}
    explicit id_gen_failed_error(const char* string) : runtime_error(string) {}
    explicit id_gen_failed_error(const runtime_error& rt) : runtime_error(rt) {}
};

class insufficient_balance_error : public std::runtime_error
{
public:
    explicit insufficient_balance_error(const std::string& basic_string) : runtime_error(basic_string) {}
    explicit insufficient_balance_error(const char* string) : runtime_error(string) {}
    explicit insufficient_balance_error(const runtime_error& rt) : runtime_error(rt) {}
};
}
}  // namespace hakurei::core