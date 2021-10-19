#include "admin_page.h"

#include <QBoxLayout>

namespace hakurei::ui
{
using namespace core::service;

admin_page::admin_page(QWidget* parent)
    : DFrame(parent)
{
    auto layout = new QHBoxLayout;
    _page_selector = new DListView(this);
    _page_selector->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addWidget(_page_selector);

    _pages = new DStackedWidget(this);
    _pages->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(_pages);
}

void admin_page::update_injector(core::service::service_injector* ij)
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

    // TODO: refresh UI data
}

void admin_page::update()
{
}
}
