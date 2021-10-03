#pragma once

#include <cstdint>
#include <memory>

namespace hakurei::core
{
namespace service
{
using auth_token = std::uint64_t;

class auth_service
{
public:
    
private:
    struct impl;
    std::unique_ptr<impl> _pImpl;
};
}
}  // namespace hakurei::core