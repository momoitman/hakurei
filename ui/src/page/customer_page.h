#pragma once

#include "page/trivial_pages.h"
#include "page/item_pages.h"
#include "widget/info_bar_widget.h"
#include "model/item_list_model.h"
#include "model/item_list_delegate.h"
#include "model/table_models.h"
#include "window/register_or_edit_window.h"
#include "service/services.h"

#include <DListView>
#include <DWidget>
#include <DStackedWidget>
#include <DFrame>
#include <DTableView>
#include <DSearchEdit>
#include <QTimer>

DWIDGET_USE_NAMESPACE;

namespace hakurei::ui
{
namespace customer_subpages
{
class my_subpage;
class discover_subpage;
}  // namespace customer_subpages
class customer_page : public DFrame
{
    Q_OBJECT
public:
    explicit customer_page(QWidget* parent);
    ~customer_page() override = default;

public slots:
    void update_injector(core::service::service_injector* ij);
    void update(core::service::auth_token token);

private slots:
    void on_switch_subpage(QModelIndex const& qidx);
    void on_request_deposit(int amount_cents);
    void on_open_deposit_page();
    void on_open_edit_user_page();
    void show_user_by_id(std::string_view id);
    void show_item_by_id(std::string_view id, bool purchase_enabled, bool purchase_visible, bool delete_enabled);
    void show_item(const item* item, bool purchase_enabled, bool purchase_visible, bool delete_enabled);
    void purchase_item(std::string_view item_id);
    void edit_user(QString const& username, QString const& password,
                   QString const& contact, QString const& address);

private:
    DListView* _page_selector;
    DStackedWidget* _pages;

    core::service::service_injector* _ij = nullptr;
    core::service::auth_service* _auth_svc = nullptr;
    core::service::item_service* _item_svc = nullptr;
    core::service::order_service* _order_svc = nullptr;
    core::service::auth_token _token = 0;
    user _current_user;

    customer_subpages::my_subpage* _my_subpage;
    customer_subpages::discover_subpage* _discover_subpage;

    item_show_page* _item_page;
    user_page* _user_page;
    deposit_page* _deposit_page;
    register_or_edit_window* _edit_window;
    static constexpr int _my_subpage_idx = 0, _discover_subpage_idx = 1;

    friend class customer_subpages::my_subpage;
    friend class customer_subpages::discover_subpage;
};

namespace customer_subpages
{
class my_subpage : public DFrame
{
    Q_OBJECT
public:
    explicit my_subpage(customer_page* parent);
    ~my_subpage() override = default;

public slots:
    void on_double_click_orders_table(const QModelIndex& index);
    void update(core::service::auth_token token);

private:
    customer_page *_pg;
    info_bar_widget* _info_bar;
    DSuggestButton* _deposit_button;
    DPushButton* _edit_user_button;

    QTableView* _my_orders_table;
    model::order_table_model* _my_orders_model;

    std::vector<order> _my_orders;
};

class discover_subpage : public DFrame
{
    Q_OBJECT
public:
    explicit discover_subpage(customer_page* parent);
    ~discover_subpage() override = default;

public slots:
    void search_text_changed();
    void update(core::service::auth_token token);
    void show_item(const QModelIndex& index);

private:
    void re_search();

    customer_page *_pg;
    DSearchEdit* _search_edit;
    std::vector<item> _items;
    QTimer* _throttler;

    QListView* _search_result;
    model::item_list_model* _my_order_model;
};
}
}