#pragma once

#include "service/services.h"
#include "widget/main_toolbar.h"
#include "login_window.h"
#include "page/customer_page.h"
#include "page/seller_page.h"
#include "page/admin_page.h"

#include <DApplication>
#include <DMainWindow>
#include <DDialog>
#include <DWidgetUtil>
#include <DApplicationSettings>
#include <DTitlebar>
#include <DProgressBar>
#include <DShadowLine>
#include <DFontSizeManager>
#include <DStackedWidget>

#include <memory>
#include <fruit/fruit.h>

DWIDGET_USE_NAMESPACE

namespace hakurei::ui
{
class main_window : public DMainWindow
{
    Q_OBJECT
public:
    explicit main_window();
    ~main_window() override {}
public slots:
    void on_login(QString const& username, QString const& password);
    void on_register(
        QString const& username, QString const& password,
        QString const& contact, QString const& address);
    void on_logout();

    void on_switch_customer_page();
    void on_switch_seller_page();
    void on_switch_admin_page();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    // Implemented and specialized in .cpp
    template <class func>
    void register_login_trycatch(func block);
    void refresh_after_token_changed();

    main_toolbar* _toolbar;
    DShadowLine* _title_line;
    DStackedWidget* _pages;

    login_window* _login_window;

    customer_page* _customer_pg;
    seller_page* _seller_pg;
    admin_page* _admin_pg;

    std::unique_ptr<core::service::service_injector> _injector;
    core::service::auth_service* _auth_svc = nullptr;
    core::service::auth_token _auth_token = 0;
};
}
