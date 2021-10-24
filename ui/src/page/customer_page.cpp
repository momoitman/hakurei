#include "customer_page.h"

#include <QStandardItemModel>
#include <QBoxLayout>

namespace hakurei::ui
{
using namespace core::service;

customer_page::customer_page(QWidget* parent)
    : DFrame(parent)
{
    _pages = new DStackedWidget(this);
    _pages->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _my_subpage = new customer_subpages::my_subpage(this);
    _discover_subpage = new customer_subpages::discover_subpage(this);
    _pages->addWidget(_my_subpage);
    _pages->addWidget(_discover_subpage);

    auto layout = new QHBoxLayout;
    setLayout(layout);
    layout->setSpacing(20);
    _page_selector = new DListView(this);
    _page_selector->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    _page_selector->setFixedWidth(200);
    layout->addWidget(_page_selector);
    layout->addWidget(_pages);

    auto model = new QStandardItemModel(_page_selector);
    _page_selector->setModel(model);

    model->setItem(_my_subpage_idx, new QStandardItem(QIcon(":images/icon_user.svg"), tr("我的")));
    model->setItem(_discover_subpage_idx, new QStandardItem(QIcon(":images/icon_search.svg"), tr("发现")));
    _page_selector->setIconSize(QSize(16, 16));
    _page_selector->setSelectionMode(QAbstractItemView::SingleSelection);

    _page_selector->setCurrentIndex(model->index(0, 0));
    _pages->setCurrentIndex(0);

    _my_subpage->update();

    connect(_page_selector->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &customer_page::on_switch_subpage);
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
    if (_pages->currentWidget() == _my_subpage)
        _my_subpage->update();
    else if (_pages->currentWidget() == _discover_subpage)
        _discover_subpage->update();
}

void customer_page::on_switch_subpage(QModelIndex const& qidx)
{
    switch (qidx.row())
    {
    case _my_subpage_idx:
        _my_subpage->update();
        _pages->setCurrentWidget(_my_subpage);
        return;
    case _discover_subpage_idx:
        _discover_subpage->update();
        _pages->setCurrentWidget(_discover_subpage);
    }
}

namespace customer_subpages
{
my_subpage::my_subpage(customer_page* parent)
    : DFrame(parent)
{
    setContentsMargins(20, 20, 20, 20);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(5);
    
    _info_bar = new info_bar_widget(this);
    _info_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->addWidget(_info_bar);
    _info_bar->updateContent("aaaaaaaaaaaaa");
    _info_bar->updateUsername("bbbbbbbbbbbbb");

    layout->addSpacing(20);

    auto my_order_header_label = new DLabel(tr("我的订单"), this);
    my_order_header_label->setStyleSheet("font: 18pt;");
    layout->addWidget(my_order_header_label);

    _my_order_table = new DTableView(this);
    _my_order_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(_my_order_table);
}

void my_subpage::update()
{
}

discover_subpage::discover_subpage(customer_page* parent)
    : DFrame(parent)
{
    setContentsMargins(20, 20, 20, 20);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(20);

    _search_edit = new DSearchEdit(this);
    _search_edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->addWidget(_search_edit);

    _search_result = new QTableView(this);
    _search_result->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(_search_result);
}

void discover_subpage::update()
{
}
}
}
