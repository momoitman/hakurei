#include "customer_page.h"

#include "common_exception_handle.h"
#include "model/cell_delegates.h"
#include "util/str_util.h"

#include <QStandardItemModel>
#include <QHeaderView>
#include <QBoxLayout>
#include <date/date.h>

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

    _item_page = new item_customer_page(this);
    _item_page->setVisible(false);
    
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
}

void customer_page::update(core::service::auth_token token)
{
    _token = token;
    
    if (_pages->currentWidget() == _my_subpage)
        _my_subpage->update(_token);
    else if (_pages->currentWidget() == _discover_subpage)
        _discover_subpage->update(_token);
}

void customer_page::on_switch_subpage(QModelIndex const& qidx)
{
    switch (qidx.row())
    {
    case _my_subpage_idx:
        _my_subpage->update(_token);
        _pages->setCurrentWidget(_my_subpage);
        return;
    case _discover_subpage_idx:
        _discover_subpage->update(_token);
        _pages->setCurrentWidget(_discover_subpage);
    }
}

void customer_page::show_item_by_id(std::string_view id, bool purchase_enabled, bool delete_enabled)
{
    common_exception_handling(
        [&]()
        {
            auto i = _item_svc->get_item(id);
            if (!i.has_value())
                return;
            show_item(&i.value(), purchase_enabled, delete_enabled);
        });
}

void customer_page::show_item(const item* item, bool purchase_enabled, bool delete_enabled)
{
    _item_page->update(*item, purchase_enabled, delete_enabled);
    _item_page->show();
}

namespace customer_subpages
{
my_subpage::my_subpage(customer_page* parent)
    : DFrame(parent), _pg(parent)
{
    setContentsMargins(20, 20, 20, 20);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(5);
    
    _info_bar = new info_bar_widget(this);
    _info_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->addWidget(_info_bar);

    layout->addSpacing(20);

    auto my_order_header_label = new DLabel(tr("我的订单"), this);
    my_order_header_label->setStyleSheet("font: 18pt;");
    layout->addWidget(my_order_header_label);

    _my_orders_table = new QTableView(this);
    _my_orders_model = new model::order_table_model(_my_orders_table);

    _my_orders_table->setModel(_my_orders_model);
    _my_orders_table->setItemDelegateForColumn(
        _my_orders_model->column_index_price_cents(),
        new model::two_precision_cell_delegate(_my_orders_table));
    _my_orders_table->setItemDelegateForColumn(
        _my_orders_model->column_index_time(),
        new model::datetime_cell_delegate(_my_orders_table));

    _my_orders_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _my_orders_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _my_orders_table->setSelectionMode(QAbstractItemView::SingleSelection);
    _my_orders_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    _my_orders_table->horizontalHeader()->resizeSection(0, 100);
    _my_orders_table->horizontalHeader()->resizeSection(1, 100);
    _my_orders_table->horizontalHeader()->resizeSection(2, 100);
    _my_orders_table->horizontalHeader()->resizeSection(3, 200);
    _my_orders_table->horizontalHeader()->resizeSection(4, 100);
    _my_orders_table->horizontalHeader()->resizeSection(5, 100);

    layout->addWidget(_my_orders_table);
}

void my_subpage::update(core::service::auth_token token)
{
    if (!_pg->_auth_svc)
        return;
    common_exception_handling(
        [&]()
        {
            common_exception_handling(
                [&]()
                {
                    _pg->_order_svc->get_my_bought_orders(token, _my_orders);
                });
            _info_bar->updateUsername(QString::fromStdString(_pg->_auth_svc->get_user_name(token)));
            _pg->_auth_svc->get_user_name(token);


            int balance_cents = _pg->_auth_svc->get_user_balance_cents(token);
            int total_cents = std::accumulate(
                _my_orders.begin(),
                _my_orders.end(),
                0,
                [](int las, order const& it) { return las + it.price_cents(); });
            auto msg = tr("您帐户的余额为 %3，您在本平台有 %1 笔订单，总消费金额 ￥%2.")
                .arg(_my_orders.size());
            msg = arg_price_cents(msg, total_cents);
            msg = arg_price_cents(msg, balance_cents);
            
            _info_bar->updateContent(msg);
            _my_orders_model->populate(_my_orders);
        });
}

void my_subpage::resizeEvent(QResizeEvent* event)
{
    DFrame::resizeEvent(event);
}

discover_subpage::discover_subpage(customer_page* parent)
    : DFrame(parent), _pg(parent)
{
    setContentsMargins(20, 20, 20, 20);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setSpacing(20);

    _search_edit = new DSearchEdit(this);
    _search_edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    layout->addWidget(_search_edit);

    _search_result = new QListView(this);
    _my_order_model = new model::item_list_model(_search_result);
    _search_result->setModel(_my_order_model);
    _search_result->setItemDelegate(new item_list_delegate(_search_result));
    _search_result->setSelectionMode(QAbstractItemView::SingleSelection);

    _search_result->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(_search_result);
    
    _throttler = new QTimer(this);
    connect(_search_edit, &DSearchEdit::textChanged, this, &discover_subpage::search_text_changed);
    connect(_throttler, &QTimer::timeout, this, &discover_subpage::re_search);
    connect(_search_result, &QListView::doubleClicked, this, &discover_subpage::show_item);
}

void discover_subpage::search_text_changed()
{
    _throttler->start(1000);
}

void discover_subpage::update(core::service::auth_token token)
{
    re_search();
}

void discover_subpage::show_item(const QModelIndex& index)
{
    auto item = _my_order_model->get_item(index);
    if (item != nullptr)
        emit _pg->show_item(item, true, false);
}

void discover_subpage::re_search()
{
    if (!_pg->_auth_svc || !_pg->_token)
        return;
    common_exception_handling(
        [&]()
        {
            _pg->_item_svc->search_item(_pg->_token, _search_edit->text().toStdString(), _items);
            _my_order_model->populate(std::move(_items));
        });
}
}
}
