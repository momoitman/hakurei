#include "service/services.h"

#include "auth_service_intl.h"
#include "item_service_intl.h"
#include "item_service_intl2.h"
#include "order_service_intl.h"

#include "model/repository.h"
#include "util/setting.h"
#include "util/password_hasher.h"

#include "persistence/csv_persistence.h"
#include "persistence/basic_search_engine.h"

namespace hakurei::core
{
namespace service
{
static bool csv_persistence_registered = persistence::register_csv_persistence();
static bool basic_search_engine_registered = persistence::register_basic_search_engine();

service_component get_services_component()
{
    // We have to hard-wire those registrations here
    // because auto-registration don't work when
    // the code is in a static library.
    // See: https://www.cppstories.com/2018/02/static-vars-static-lib/
    if (!csv_persistence_registered || !basic_search_engine_registered)
        std::terminate();
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