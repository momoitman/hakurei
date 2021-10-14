#include "id_generator.h"

#include <cmath>
#include <charconv>
#include <fmt/core.h>

#include "service/service_exceptions.h"

namespace hakurei::core
{
namespace service
{
#define id_num_length 5
#define xstr(a) str(a)
#define str(a) #a

std::string generate_next_id(std::string_view previous)
{
    if (previous.size() != id_num_length + 1)
        throw id_gen_failed_error(std::string(previous), 
            id_gen_failed_error::bad_last_id);
    char prefix = previous[0];
    int previous_val = 0;
    auto parse_result = std::from_chars(
        previous.data() + 1, previous.data() + previous.size(),
        previous_val);
    if (parse_result.ec != std::errc() 
        || parse_result.ptr != previous.data() + previous.size())
        throw id_gen_failed_error(std::string(previous), 
            id_gen_failed_error::bad_last_id);
    if (previous_val < 0)
        throw id_gen_failed_error(std::string(previous),
                                  id_gen_failed_error::bad_last_id);
    int val = previous_val + 1;
    if (val >= std::pow(10, id_num_length))
        throw id_gen_failed_error(std::string(previous),
                                  id_gen_failed_error::overflow);
    return fmt::format("{:c}{:0" xstr(id_num_length) "d}", prefix, val);
}
}
}  // namespace hakurei::core