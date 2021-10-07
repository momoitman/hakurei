#pragma once

#include "password_hasher.h"

namespace hakurei::core
{
namespace util
{
namespace
{
const size_t hash_len = 32;
const size_t salt_len = 16;
const int t_cost = 2;
const int m_cost = 1 << 16;  // 64 MB
const int parallelism = 16;
const int hash_encoded_max_len = 256;
}  // namespace

class argon2_password_hasher : public password_hasher
{
public:
    INJECT(argon2_password_hasher()) {}
    ~argon2_password_hasher() override = default;
    std::string hash_password(std::string_view password) override;
    bool verity_password(std::string_view password, std::string_view hash) override;
};

}  // namespace util
}  // namespace hakurei::core