#pragma once

#include <DListView>
#include <DWidget>
#include <DStackedWidget>
#include <DFrame>
#include <service/services.h>

DWIDGET_USE_NAMESPACE;

namespace hakurei::ui
{
class admin_page : public DFrame {
    Q_OBJECT

public:
    explicit admin_page(QWidget* parent);
    ~admin_page() override = default;

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
};
}