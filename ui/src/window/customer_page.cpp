
#include "customer_page.h"

#include <QStandardItemModel>
#include <QBoxLayout>

namespace hakurei::ui
{
using namespace core::service;

customer_page::customer_page(QWidget* parent)
    : DFrame(parent)
{
    auto layout = new QHBoxLayout;
    setLayout(layout);
    layout->setSpacing(20);
    _page_selector = new DListView(this);
    _page_selector->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    _page_selector->setFixedWidth(200);
    layout->addWidget(_page_selector);

    _pages = new DStackedWidget(this);
    _pages->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(_pages);

    auto model = new QStandardItemModel(_page_selector);
    _page_selector->setModel(model);

    model->setItem(0, new QStandardItem(QIcon(":images/icon_user.svg"), tr("我的")));
    model->setItem(1, new QStandardItem(QIcon(":images/icon_search.svg"), tr("发现")));
    _page_selector->setIconSize(QSize(16, 16));

    _pages->setCurrentIndex(0);
}

void customer_page::update_injector(core::service::service_injector* ij)
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

void customer_page::update()
{
}
}
