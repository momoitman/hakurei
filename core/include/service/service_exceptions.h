#pragma once
#include "internal_exceptions.h"

#include <cassert>
#include <stdexcept>

namespace hakurei::core
{
namespace service
{
class duplication_error : public hakurei_error
{
public:
    explicit duplication_error(const std::string& basic_string) : hakurei_error(basic_string) { }
};

class invalid_token_error : public hakurei_error
{
public:
    explicit invalid_token_error()
        : hakurei_error("The auth token provided is invalid!") {}
};

class invalid_credential_error : public hakurei_error
{
public:
    explicit invalid_credential_error()
        : hakurei_error("Invalid username or password!") {}
    explicit invalid_credential_error(const std::string& basic_string) : hakurei_error(basic_string) {}
};

class not_found_error : public hakurei_error
{
public:
    explicit not_found_error(const std::string& basic_string) : hakurei_error(basic_string) {}
};

class bad_status_error : public hakurei_error
{
public:
    explicit bad_status_error(const std::string& basic_string) : hakurei_error(basic_string) {}
};

class access_denied_error : public hakurei_error
{
public:
    explicit access_denied_error(const std::string& msg)
        : hakurei_error(fmt::format("Access denied: {}", msg)) {}
};

class illegal_string_error : public hakurei_error
{
public:
    explicit illegal_string_error(std::string const& reason) : hakurei_error(reason) { }
};

class id_gen_failed_error : public hakurei_error
{
public:
    enum fail_type : uint8_t
    {
        bad_last_id = 1,
        overflow = 2
    };
    explicit id_gen_failed_error(std::string const& last, fail_type type)
        : hakurei_error(gen_msg(last, type)), _last(last), _type(type) {}

    [[nodiscard]] std::string& last() { return _last; }
    [[nodiscard]] fail_type& type() { return _type; }

private:
    std::string _last;
    fail_type _type;
    static std::string gen_msg(std::string const& last, fail_type type)
    {
        switch (type)
        {
        case bad_last_id:
            return fmt::format("Failed to generate next id from malformed last id {}.", last);
        case overflow:
            return fmt::format("Failed to generate next id from last id {}, too much elements.", last);
        }
        assert(false && "Bad id_gen_failed_error type!");
        return "";
    }
};

class insufficient_balance_error : public hakurei_error
{
public:
    explicit insufficient_balance_error(
        int required_cents,
        int owned_cents)
        : hakurei_error(
            fmt::format("Not enough balance in account. Owned: {}, Required: {}",
                owned_cents / 100.0, required_cents / 100.0)),
                _required(required_cents), _owned(owned_cents){}

    [[nodiscard]] int required_cents() { return _required; }
    [[nodiscard]] int owned_cents() { return _owned; }
private:
    int _required, _owned;
};
}
}  // namespace hakurei::core