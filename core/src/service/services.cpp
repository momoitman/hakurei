#include "services.h"

#include "auth_service_intl.h"
#include "item_service_intl.h"
#include "item_service_intl2.h"
#include "order_service_intl.h"

#include "model/repository.h"
#include "util/setting.h"
#include "util/password_hasher.h"

namespace hakurei::core
{
namespace service
{
service_component get_services_component()
{
    return fruit::createComponent()
        .bind<item_service_intl, item_service_impl>()
        .bind<item_service_intl2, item_service_impl2>()  // very dirty here
        .bind<auth_service_intl, auth_service_impl>()
        .bind<order_service_intl, order_service_impl>()
        .bind<item_service, item_service_impl>()
        .bind<auth_service, auth_service_impl>()
        .bind<order_service, order_service_impl>()
        .install(get_setting_component)
        .install(model::get_repository_component)
        .install(util::get_password_hasher_component)
    ;
}
}
}  // namespace hakurei::core