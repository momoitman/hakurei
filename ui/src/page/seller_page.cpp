#include "seller_page.h"

#include "common_exception_handle.h"
#include "util/str_util.h"
#include "model/cell_delegates.h"

#include <QBoxLayout>
#include <QHeaderView>

namespace hakurei::ui
{
using namespace core::service;
using namespace model;

seller_page::seller_page(QWidget* parent)
    : DFrame(parent)
{
    auto vLayout = new QVBoxLayout(this);
    vLayout->setSpacing(5);
    _info_bar = new info_bar_widget(this);

    _add_item_button = new DSuggestButton(tr("上架商品"), this);

    auto downpane = new QWidget(this);
    auto downlayout = new QGridLayout(downpane);

    vLayout->addWidget(_info_bar);
    vLayout->addWidget(_add_item_button);
    vLayout->addWidget(downpane);

    auto my_item_header_label = new DLabel(tr("在架商品"), this);
    auto my_order_header_label = new DLabel(tr("已售出订单"), this);
    my_item_header_label->setStyleSheet("font: 18pt;");
    my_order_header_label->setStyleSheet("font: 18pt;");

    _my_item_table = new QTableView(this);
    _my_order_table = new QTableView(this);

    my_item_header_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    my_order_header_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _my_item_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _my_order_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    downlayout->addWidget(my_item_header_label, 0, 0);
    downlayout->addWidget(my_order_header_label, 0, 1);
    downlayout->addWidget(_my_item_table, 1, 0);
    downlayout->addWidget(_my_order_table, 1, 1);

    _item_table_model = new item_table_model(this);
    _order_table_model = new order_table_model(this);
    _my_item_table->setModel(_item_table_model);
    _my_order_table->setModel(_order_table_model);

    _my_order_table->setSelectionMode(QAbstractItemView::SingleSelection);
    _my_item_table->setSelectionMode(QAbstractItemView::SingleSelection);
    _my_order_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _my_item_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _my_order_table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    _my_item_table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    item_table_model::configure_table(_my_item_table);
    order_table_model::configure_table(_my_order_table);

    _item_show_page = new item_show_page(this);
    _user_page = new user_page(this);
    _item_edit_page = new item_edit_page(this);

    connect(_add_item_button, &DSuggestButton::clicked, this, &seller_page::on_add_item_click);
    connect(_my_order_table, &QTableView::doubleClicked, this, &seller_page::on_order_table_double_click);
    connect(_my_item_table, &QTableView::doubleClicked, this, &seller_page::on_item_table_double_click);
    connect(_item_edit_page, &item_edit_page::on_create_item, this, &seller_page::create_item);
    connect(_item_edit_page, &item_edit_page::on_edit_item, this, &seller_page::edit_item);
    connect(_item_edit_page, &item_edit_page::on_delete_item, this, &seller_page::delete_item);
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
    _token = 0;
}

void seller_page::update(core::service::auth_token token)
{
    _token = token;
    if (!_token)
        return;
    common_exception_handling(
        [&]()
        {
            _item_svc->get_my_items(_token, _my_items);
            _order_svc->get_my_sold_orders(_token, _my_orders);
            _info_bar->updateUsername(QString::fromStdString(_auth_svc->get_user_name(token)));
            std::reverse(_my_orders.begin(), _my_orders.end());

            int total_cents = std::accumulate(
                _my_orders.begin(),
                _my_orders.end(),
                0,
                [](int las, order const& it) { return las + it.price_cents(); });
            auto msg = tr("您当前有 %1 件商品在架，有 %2 笔已售订单，总售出金额 ￥%3.")
                           .arg(_my_items.size())
                           .arg(_my_orders.size());
            msg = arg_price_cents(msg, total_cents);
            
            _info_bar->updateContent(msg);
            _order_table_model->populate(_my_orders);
            _item_table_model->populate(_my_items);
        });
}

void seller_page::on_add_item_click()
{
    _item_edit_page->update_new();
    _item_edit_page->show();
}

void seller_page::on_item_table_double_click(QModelIndex const& index)
{
    show_edit_item(_item_table_model->get_item(index));
}

void seller_page::on_order_table_double_click(QModelIndex const& index)
{
    switch (index.column())
    {
    case model::order_table_model::column_item_id:
        show_item_by_id(_order_table_model->get_item(index)->item_id());
        break;
    case model::order_table_model::column_customer_id:
        show_user_by_id(_order_table_model->get_item(index)->customer_uid());
        break;
    case model::order_table_model::column_seller_id:
        show_user_by_id(_order_table_model->get_item(index)->seller_uid());
        break;
    }
}

void seller_page::show_item_by_id(std::string_view id)
{
    common_exception_handling(
        [&]()
        {
            auto i = _item_svc->get_item(id);
            if (!i.has_value())
                return;
            show_item(&i.value());
        });
}

void seller_page::show_item(model::item* item)
{
    _item_show_page->update(*item, false, false, false);
    _item_show_page->show();
}

void seller_page::show_edit_item(model::item const* item)
{
    _item_edit_page->update(*item, item->status() == item_status::on_stock);
    _item_edit_page->show();
}

void seller_page::show_user_by_id(std::string user_id)
{
    common_exception_handling(
        [&]()
        {
            auto u = _auth_svc->get_user_info(user_id);
            if (!u)
                return;
            _user_page->update(u.value(), false, false);
            _user_page->show();
        });
}

void seller_page::create_item(QString const& name, int price_cents, QString const& desc)
{
    common_exception_handling(
        [&]()
        {
            auto name_std = name.toStdString();
            auto desc_std = desc.toStdString();
            _item_svc->add_item(
                _token, name_std, price_cents, desc_std);
            update(_token);
            _item_edit_page->hide();
        });
}

void seller_page::edit_item(std::string_view item_id, QString const& name, int price_cents, QString const& desc)
{
    common_exception_handling(
        [&]()
        {
            auto i = _item_svc->get_item(item_id);
            if (!i)
                return;
            auto name_std = name.toStdString();
            auto desc_std = desc.toStdString();
            _item_svc->set_item(
                _token, item_id,
                name_std == i->name() ? std::nullopt : std::optional(name_std),
                price_cents == i->price_cents() ? std::nullopt : std::optional(price_cents),
                desc_std == i->description() ? std::nullopt : std::optional(desc_std));
            update(_token);
            _item_edit_page->hide();
        });
}

void seller_page::delete_item(std::string_view item_id)
{
    common_exception_handling(
        [&]()
        {
            _item_svc->remove_item(_token, item_id);
            update(_token);
            _item_edit_page->hide();
        });
}
}
