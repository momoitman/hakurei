#pragma once

#include "service/auth_service.h"
#include "service/item_service.h"
#include "service/order_service.h"

#include <fruit/fruit.h>

namespace hakurei::core
{
namespace service
{
using service_injector = fruit::Injector<auth_service, order_service, item_service>;
using service_component = fruit::Component<auth_service, order_service, item_service>;
service_component get_services_component();

}
}  // namespace hakurei::core