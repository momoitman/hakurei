#pragma once

#include <stdexcept>
#include <fmt/core.h>

namespace hakurei::core
{
class hakurei_error
{
public:
    hakurei_error(std::string const& reason): _reason(reason) {}
    [[nodiscard]] std::string const& reason() const { return _reason; }

private:
    std::string _reason;
};

class persistence_error : public hakurei_error
{
public:
    explicit persistence_error(const std::string& msg)
        : hakurei_error(fmt::format("Persistence Error: {}", msg)) {}
};

class factory_not_found : public hakurei_error
{
public:
    explicit factory_not_found(const std::string& name, const std::string& reg_name)
        : hakurei_error(
            fmt::format("Provider {} for registry {} is not found. Check it in your config file!",
                name, reg_name)
        ) {}
};

class password_hash_error : public hakurei_error
{
public:
    explicit password_hash_error(const std::string& basic_string)
        : hakurei_error(fmt::format("hakurei::core::password_hash_error: {}", basic_string)) {}
};
}