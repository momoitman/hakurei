#pragma once

#include "widget/info_bar_widget.h"
#include <service/services.h>

#include <DListView>
#include <DWidget>
#include <DStackedWidget>
#include <DFrame>
#include <DTableView>
#include <DSearchEdit>

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
    void update();

private slots:
    void on_switch_subpage(QModelIndex const& qidx);

private:
    DListView* _page_selector;
    DStackedWidget* _pages;

    core::service::service_injector* _ij = nullptr;
    core::service::auth_service* _auth_svc = nullptr;
    core::service::item_service* _item_svc = nullptr;
    core::service::order_service* _order_svc = nullptr;

    customer_subpages::my_subpage* _my_subpage;
    customer_subpages::discover_subpage* _discover_subpage;
    static constexpr int _my_subpage_idx = 0, _discover_subpage_idx = 1;
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
    void update();

private:
    info_bar_widget* _info_bar;
    DTableView* _my_order_table;
};

class discover_subpage : public DFrame
{
    Q_OBJECT
public:
    explicit discover_subpage(customer_page* parent);
    ~discover_subpage() override = default;

public slots:
    void update();

private:
    DSearchEdit* _search_edit;
    DTableView* _search_result;
};
}
}