#include "customer_page.h"

#include "common_exception_handle.h"
#include "model/cell_delegates.h"
#include "util/str_util.h"

#include <QStandardItemModel>
#include <QHeaderView>
#include <QBoxLayout>
#include <algorithm>
#include <date/date.h>

namespace hakurei::ui
{
using namespace core::service;

customer_page::customer_page(QWidget* parent)
    : DFrame(parent)
{
    auto layout = new QHBoxLayout;
    setLayout(layout);
    layout->setSpacing(20);

    _pages = new DStackedWidget(this);
    _pages->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _my_subpage = new customer_subpages::my_subpage(this);
    _discover_subpage = new customer_subpages::discover_subpage(this);
    _pages->addWidget(_my_subpage);
    _pages->addWidget(_discover_subpage);

    _page_selector = new DListView(this);
    _page_selector->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    _page_selector->setFixedWidth(200);

    layout->addWidget(_page_selector);
    layout->addWidget(_pages);

    auto model = new QStandardItemModel(_page_selector);
    _page_selector->setModel(model);
    _page_selector->setIconSize(QSize(16, 16));
    _page_selector->setSelectionMode(QAbstractItemView::SingleSelection);

    model->setItem(_my_subpage_idx, new QStandardItem(QIcon(":images/icon_user.svg"), tr("我的")));
    model->setItem(_discover_subpage_idx, new QStandardItem(QIcon(":images/icon_search.svg"), tr("发现")));

    _page_selector->setCurrentIndex(model->index(0, 0));
    _pages->setCurrentIndex(0);

    _item_page = new item_show_page(this);
    _user_page = new user_page(this);
    _deposit_page = new deposit_page(this);
    _edit_window = new register_or_edit_window(this);
    
    connect(_page_selector->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &customer_page::on_switch_subpage);
    connect(_deposit_page, &deposit_page::on_request_deposit, this, &customer_page::on_request_deposit);
    connect(_item_page, &item_show_page::on_purchase_item, this, &customer_page::purchase_item);
    connect(_edit_window, &register_or_edit_window::on_register, this, &customer_page::edit_user);
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

void customer_page::on_request_deposit(int amount_cents)
{
    common_exception_handling(
        [&]()
        {
            if (!_auth_svc || !_token)
                return;
            _auth_svc->deposit_money(_token, amount_cents);
            _deposit_page->hide();
            _my_subpage->update(_token);
        });
}

void customer_page::on_open_deposit_page()
{
    _deposit_page->reset_and_show();
}

void customer_page::on_open_edit_user_page()
{
    _edit_window->reset_and_show(_current_user);
}

void customer_page::show_user_by_id(std::string_view id)
{
    common_exception_handling(
        [&]()
        {
            auto u = _auth_svc->get_user_info(id);
            if (!u.has_value())
                return;
            _user_page->update(*u, false, false);
            _user_page->show();
        });
}

void customer_page::show_item_by_id(std::string_view id, bool purchase_enabled, bool purchase_visible, bool delete_enabled)
{
    common_exception_handling(
        [&]()
        {
            auto i = _item_svc->get_item(id);
            if (!i.has_value())
                return;
            show_item(&i.value(), purchase_enabled, purchase_visible, delete_enabled);
        });
}

void customer_page::show_item(const item* item, bool purchase_enabled, bool purchase_visible, bool delete_enabled)
{
    _item_page->update(*item, purchase_visible, purchase_enabled, delete_enabled);
    _item_page->show();
}

void customer_page::purchase_item(std::string_view item_id)
{
    common_exception_handling(
        [&]()
        {
            if (!_auth_svc || !_token)
                return;

            _order_svc->place_order(_token, item_id);
            _item_page->hide();
            _page_selector->setCurrentIndex(_page_selector->model()->index(0, 0));
        });
}

void customer_page::edit_user(QString const& username, QString const& password, QString const& contact, QString const& address)
{
    auto username_std = username.toStdString();
    auto password_std = password.toStdString();
    auto contact_std = contact.toStdString();
    auto address_std = address.toStdString();

    common_exception_handling(
        [&]()
        {
            _auth_svc->set_user_info(
                _token,
                username_std == _current_user.name() ? std::nullopt : std::optional(username_std),
                password_std.empty() ? std::nullopt : std::optional(password_std),
                contact_std == _current_user.contact() ? std::nullopt : std::optional(contact_std),
                address_std == _current_user.address() ? std::nullopt : std::optional(address_std)
            );
            _edit_window->hide();
            _my_subpage->update(_token);
        });
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

    QWidget* button_grp = new QWidget(this);
    _deposit_button = new DSuggestButton(tr("充值"), button_grp);
    _edit_user_button = new DSuggestButton(tr("修改信息"), button_grp);
    QHBoxLayout* button_layout = new QHBoxLayout(button_grp);
    button_layout->addWidget(_deposit_button, 50);
    button_layout->addWidget(_edit_user_button, 50);
    layout->addWidget(button_grp);

    layout->addSpacing(20);

    auto my_order_header_label = new DLabel(tr("我的订单"), this);
    my_order_header_label->setStyleSheet("font: 18pt;");
    layout->addWidget(my_order_header_label);

    _my_orders_table = new QTableView(this);
    layout->addWidget(_my_orders_table);

    _my_orders_model = new model::order_table_model(_my_orders_table);
    _my_orders_table->setModel(_my_orders_model);

    _my_orders_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _my_orders_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _my_orders_table->setSelectionMode(QAbstractItemView::SingleSelection);
    _my_orders_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    model::order_table_model::configure_table(_my_orders_table);

    connect(_my_orders_table, &QTableView::doubleClicked, 
        this, &my_subpage::on_double_click_orders_table);
    connect(_deposit_button, &DSuggestButton::clicked,
            _pg, &customer_page::on_open_deposit_page);
    connect(_edit_user_button, &DPushButton::clicked,
            _pg, &customer_page::on_open_edit_user_page);
}

void my_subpage::on_double_click_orders_table(const QModelIndex& index)
{
    switch (index.column())
    {
    case model::order_table_model::column_item_id:
        _pg->show_item_by_id(_my_orders_model->get_item(index)->item_id(), 
                             false, false, false);
        break;
    case model::order_table_model::column_customer_id:
        _pg->show_user_by_id(_my_orders_model->get_item(index)->customer_uid());
        break;
    case model::order_table_model::column_seller_id:
        _pg->show_user_by_id(_my_orders_model->get_item(index)->seller_uid());
        break;
    }
}

void my_subpage::update(core::service::auth_token token)
{
    if (!_pg->_auth_svc)
        return;
    common_exception_handling(
        [&]()
        {
            _pg->_current_user = _pg->_auth_svc->get_user_info(token);
            _pg->_order_svc->get_my_bought_orders(token, _my_orders);
            _info_bar->updateUsername(QString::fromStdString(_pg->_auth_svc->get_user_name(token)));
            std::reverse(_my_orders.begin(), _my_orders.end()); // TODO performance..

            int total_cents = std::accumulate(
                _my_orders.begin(),
                _my_orders.end(),
                0,
                [](int las, order const& it) { return las + it.price_cents(); });
            auto msg = tr("您帐户的余额为 ￥%3，您在本平台有 %1 笔订单，总消费金额 ￥%2.")
                .arg(_my_orders.size());
            msg = arg_price_cents(msg, total_cents);
            msg = arg_price_cents(msg, _pg->_current_user.balance_cents());
            
            _info_bar->updateContent(msg);
            _my_orders_model->populate(_my_orders);
        });
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
        emit _pg->show_item(
            item, _pg->_current_user.balance_cents() >= item->price_cents(),
            true, false);
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
