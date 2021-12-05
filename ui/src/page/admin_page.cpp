#include "admin_page.h"

#include <QBoxLayout>
#include <QTableView>
#include <QHeaderView>
#include <QMessageBox>
#include <QMenu>

#include "common_exception_handle.h"
#include "model/cell_delegates.h"

namespace hakurei::ui
{
using namespace core::service;
using namespace admin_subpages;

admin_page::admin_page(QWidget* parent)
    : DFrame(parent)
{
    auto layout = new QHBoxLayout;
    setLayout(layout);
    layout->setSpacing(20);

    _pages = new DStackedWidget(this);
    _pages->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _user_page = new user_subpage(this);
    _item_page = new item_subpage(this);
    _order_page = new order_subpage(this);
    _pages->addWidget(_user_page);
    _pages->addWidget(_item_page);
    _pages->addWidget(_order_page);

    _page_selector = new DListView(this);
    _page_selector->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    _page_selector->setFixedWidth(200);
    _page_selector->setIconSize(QSize(16, 16));
    _page_selector->setSelectionMode(QAbstractItemView::SingleSelection);
    layout->addWidget(_page_selector);
    layout->addWidget(_pages);

    auto model = new QStandardItemModel(_page_selector);
    _page_selector->setModel(model);

    model->setItem(_user_page_idx, new QStandardItem(QIcon(":images/icon_user.svg"), tr("用户管理")));
    model->setItem(_item_page_idx, new QStandardItem(QIcon(":images/icon_item.svg"), tr("商品管理")));
    model->setItem(_order_page_idx, new QStandardItem(QIcon(":images/icon_order.svg"), tr("订单管理")));

    _page_selector->setCurrentIndex(model->index(0, 0));
    _pages->setCurrentIndex(0);

    connect(_page_selector->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &admin_page::on_switch_subpage);
}

void admin_page::update(auth_token token)
{
    _token = token;

    if (_pages->currentWidget() == _user_page)
        _user_page->update_page();
    else if (_pages->currentWidget() == _item_page)
        _item_page->update_page();
    else if (_pages->currentWidget() == _order_page)
        _order_page->update_page();
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

void admin_page::on_switch_subpage(QModelIndex const& qidx)
{
    switch (qidx.row())
    {
    case _user_page_idx:
        _user_page->update_page();
        _pages->setCurrentWidget(_user_page);
        return;
    case _item_page_idx:
        _item_page->update_page();
        _pages->setCurrentWidget(_item_page);
        return;
    case _order_page_idx:
        _order_page->update_page();
        _pages->setCurrentWidget(_order_page);
        return;
    }
}

template <class T, class This, bool remove_enabled>
abstract_subpage<T, This, remove_enabled>::abstract_subpage(admin_page* parent, This* t)
    : _t(t), _pg(parent)
{
    t->setContentsMargins(20, 20, 20, 20);
    auto layout = new QVBoxLayout(t);
    layout->setSpacing(5);

    _table_view = new QTableView(t);
    layout->addWidget(_table_view);

    _model = new model_type(_table_view);
    _table_view->setModel(_model);

    _table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _table_view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _table_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    _table_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    _table_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _table_view->setContextMenuPolicy(Qt::CustomContextMenu);

    if constexpr (remove_enabled)
    {
        _remove = new QMenu(t);
        auto remove_action = _remove->addAction(t->tr("删除"));
        t->connect(remove_action, &QAction::triggered,
                   [&]()
                   {
                       if constexpr (!remove_enabled)
                           return;
                       auto indexes = _table_view->selectionModel()->selectedRows();
                       auto str = QString();
                       for (auto& qm : indexes)
                           str += QString::fromStdString(_model->get_item(qm.row())->id()) + ' ';

                       auto conf = QMessageBox::question(
                           _t,
                           _t->tr("删除"),
                           _t->tr("确定删除 %1 个项 (%2) 吗？").arg(indexes.size()).arg(str),
                           QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
                       if (conf != QMessageBox::Yes)
                           return;

                       common_exception_handling(
                           [&]()
                           {
                               if (!_pg->_token)
                                   return;
                               for (auto& qm : indexes)
                                   _t->remove_item(_model->get_item(qm.row())->id());
                           });
                       update_page();
                   });
    }

    model_type::configure_table(_table_view);

    t->connect(_table_view, &QTableView::customContextMenuRequested,
               [&](const QPoint& pos)
               {
                   if constexpr (remove_enabled)
                       _remove->exec(_t->mapToGlobal(pos));
               });
}

template <class T, class This, bool enable_remove>
void abstract_subpage<T, This, enable_remove>::update_page()
{
    if (!_pg->_auth_svc)
        return;
    common_exception_handling(
        [&]()
        {
            _t->update_items();
            _model->populate(_items);
        });
}

user_subpage::user_subpage(admin_page* parent)
    : abstract_subpage(parent, this)
{
}

void user_subpage::update_items()
{
    _pg->_auth_svc->get_all_users(_pg->_token, _items);
}

void user_subpage::remove_item(std::string_view id)
{
    _pg->_auth_svc->remove_user(_pg->_token, id);
}

item_subpage::item_subpage(admin_page* parent)
    : abstract_subpage(parent, this)
{
}

void item_subpage::update_items()
{
    _pg->_item_svc->get_all_items(_pg->_token, _items);
}

void item_subpage::remove_item(std::string_view id)
{
    _pg->_item_svc->remove_item(_pg->_token, id);
}

order_subpage::order_subpage(admin_page* parent)
    : DFrame(parent), abstract_subpage(parent, this)
{
}

void order_subpage::update_items()
{
    _pg->_order_svc->get_all_orders(_pg->_token, _items);
}
}
