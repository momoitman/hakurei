#pragma once

#include <DListView>
#include <DWidget>
#include <DStackedWidget>
#include <DFrame>
#include <service/services.h>

DWIDGET_USE_NAMESPACE;

namespace hakurei::ui
{
class customer_page : public DFrame
{
    Q_OBJECT

public:
    explicit customer_page(QWidget* parent);
    ~customer_page() override = default;

public slots:
    void update_injector(core::service::service_injector* ij);
    void update();

private:
    DListView* _page_selector;
    DStackedWidget* _pages;

    core::service::service_injector* _ij = nullptr;
    core::service::auth_service* _auth_svc = nullptr;
    core::service::item_service* _item_svc = nullptr;
    core::service::order_service* _order_svc = nullptr;

    class my_subpage : public DFrame
    {
    public:
        explicit my_subpage(customer_page* parent);
        ~my_subpage() override = default;

    public slots:
        void update();

    private:

    };
};
}