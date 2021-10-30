#include "seller_page.h"

#include <QBoxLayout>

namespace hakurei::ui
{
using namespace core::service;

seller_page::seller_page(QWidget* parent)
    : DFrame(parent)
{

}

void seller_page::update_injector(core::service::service_injector* ij)
{
    if (!ij)
    {
        _ij = nullptr;
        _auth_svc = nullptr;
        _item_svc = nullptr;
        _order_svc = nullptr;
    }
    else
    {
        _ij = ij;
        _auth_svc = ij->get<auth_service*>();
        _item_svc = ij->get<item_service*>();
        _order_svc = ij->get<order_service*>();
    }
}

void seller_page::update(core::service::auth_token token)
{
    _token = token;
    if (!_token)
        return;

}
}
