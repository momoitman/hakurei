#include "argon2_password_hasher.h"

#include "argon2.h"

#include <algorithm>
#include <cstring>
#include <random>
#include <functional>
#include <fmt/core.h>

namespace hakurei::core
{
namespace util
{
void generate_salt(uint8_t* dest, size_t len)
{
    std::random_device r;
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 gen(seed);

    std::uniform_int_distribution ud(
        std::numeric_limits<uint8_t>::min(),
        std::numeric_limits<uint8_t>::max());

    std::generate_n(dest, len, [&] { return ud(gen); });
}

std::string argon2_password_hasher::hash_password(std::string_view password)
{
    uint8_t salt[salt_len];
    char encoded[hash_encoded_max_len];
    generate_salt(salt, salt_len);
    
    auto ret = argon2id_hash_encoded(
        t_cost, m_cost, parallelism,
        password.data(), password.size(),
        salt, salt_len,
        hash_len,
        encoded, hash_encoded_max_len
    );
    if (ret != ARGON2_OK)
        throw password_hash_error(fmt::format("argon2id_hash_encoded returned {}", ret));

    auto len = strnlen(encoded, hash_encoded_max_len);
    if (len == hash_encoded_max_len)
        throw password_hash_error("resulting string didn't terminate");

    return std::string(encoded, encoded + len);
}

bool argon2_password_hasher::verity_password(std::string_view password, std::string_view hash)
{
    if (hash.size() > hash_encoded_max_len - 1)
        throw password_hash_error(fmt::format("hash too long:{}", hash.size()));

    // Think: what if we don't do this step?
    char encoded[hash_encoded_max_len];
    strncpy(encoded, hash.data(), hash.size());
    encoded[hash.size()] = '\0';

    auto ret = argon2id_verify(hash.data(), password.data(), password.size());
    if (ret == ARGON2_VERIFY_MISMATCH)
        return false;
    if (ret == ARGON2_OK)
        return true;
    throw password_hash_error(fmt::format("argon2id_verify returned {}", ret));
}

password_hasher_component get_password_hasher_component()
{
    return fruit::createComponent()
        .bind<password_hasher, argon2_password_hasher>();
}
}
}  // namespace hakurei::core