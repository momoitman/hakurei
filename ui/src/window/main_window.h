#pragma once

#include "service/services.h"
#include "main_toolbar.h"
#include "login_window.h"

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

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    template<class func>
    void register_login_trycatch(func block);

    main_toolbar* _toolbar;
    DShadowLine* _title_line;
    DStackedWidget* _pages;

    login_window* _login_window;

    std::unique_ptr<core::service::service_injector> _injector;
    core::service::auth_service* _auth_svc = nullptr;
    core::service::auth_token _auth_token = 0;
};
}
