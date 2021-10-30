#include "main_window.h"

#include "../common_exception_handle.h"

namespace hakurei::ui
{
using namespace core::service;

main_window::main_window()
{
    common_exception_handling<void>([this]()
                                    {
                                        _injector = std::make_unique<service_injector>(get_services_component);
                                        _auth_svc = _injector->get<auth_service*>();
                                    });

    resize(1200, 800);
    titlebar()->setIcon(QIcon::fromTheme(":images/hakurei.png"));

    _pages = new DStackedWidget(this);
    _pages->setContentsMargins(20, 20, 20, 20);
    setCentralWidget(_pages);

    _customer_pg = new customer_page(this);
    _seller_pg = new seller_page(this);
    _admin_pg = new admin_page(this);
    _customer_pg->update_injector(_injector.get());
    _seller_pg->update_injector(_injector.get());
    _admin_pg->update_injector(_injector.get());

    _pages->addWidget(_customer_pg);
    _pages->addWidget(_seller_pg);
    _pages->addWidget(_admin_pg);

    _toolbar = new main_toolbar(this);
    _toolbar->setFocusPolicy(Qt::TabFocus);
    _toolbar->set_enabled(false, false);
    titlebar()->setCustomWidget(_toolbar);
    setTabOrder(titlebar(), _toolbar);

    auto menu = new DMenu(this);
    titlebar()->setMenu(menu);
    auto logout_action = new QAction(tr("注销", "注销当前登录"), menu);
    menu->addAction(logout_action);
    connect(logout_action, &QAction::triggered, this, &main_window::on_logout);

    _title_line = new DShadowLine(_pages);
    _title_line->setFixedWidth(width());
    _title_line->setFixedHeight(10);
    _title_line->move(0, 0);
    _title_line->show();

    _login_window = new login_window(nullptr);
    _login_window->reset_and_show();
    connect(_login_window, &login_window::on_login, this, &main_window::on_login);
    connect(_login_window, &login_window::on_register, this, &main_window::on_register);
    connect(_toolbar, &main_toolbar::on_tab_customer_clicked, this, &main_window::on_switch_customer_page);
    connect(_toolbar, &main_toolbar::on_tab_seller_clicked, this, &main_window::on_switch_seller_page);
    connect(_toolbar, &main_toolbar::on_tab_admin_clicked, this, &main_window::on_switch_admin_page);

    emit on_switch_customer_page();

    hide();
}

void main_window::on_login(QString const& username, QString const& password)
{
    register_login_trycatch(
        [&]()
        {
            _auth_token = _auth_svc->login_user(
                username.toStdString(), password.toStdString());
            refresh_after_token_changed();
        });
}

void main_window::on_register(
    QString const& username, QString const& password,
    QString const& contact, QString const& address)
{
    register_login_trycatch(
        [&]()
        {
            _auth_token = _auth_svc->register_user(
                username.toStdString(), password.toStdString(), 
                contact.toStdString(), address.toStdString());
            refresh_after_token_changed();
        });
}

void main_window::on_logout()
{
    _auth_svc->logout_user(_auth_token);
    _auth_token = 0;
    _toolbar->set_enabled(false, false);
    hide();
    _login_window->reset_and_show();
}

void main_window::on_switch_customer_page()
{
    if (_auth_token)
        _customer_pg->update(_auth_token);
    _pages->setCurrentWidget(_customer_pg);
}

void main_window::on_switch_seller_page()
{
    if (_auth_token)
        _seller_pg->update(_auth_token);
    _pages->setCurrentWidget(_seller_pg);
}

void main_window::on_switch_admin_page()
{
    common_exception_handling<void>(
        [&]()
        {
            if (_auth_token && _auth_svc && _auth_svc->is_user_admin(_auth_token))
            {
                _admin_pg->update();
                _pages->setCurrentWidget(_admin_pg);
            }
        });
}

void main_window::resizeEvent(QResizeEvent* event)
{
    DMainWindow::resizeEvent(event);
    _title_line->setFixedWidth(width());
}

void main_window::refresh_after_token_changed()
{
    _login_window->hide();
    _toolbar->set_enabled(true, _auth_svc->is_user_admin(_auth_token));
    _toolbar->set_username(QString::fromStdString(_auth_svc->get_user_name(_auth_token)));
    _customer_pg->update(_auth_token);
    _seller_pg->update(_auth_token);
    show();
}

template <class func>
void main_window::register_login_trycatch(func block)
{
    common_exception_handling<void>(
        [this, block]()
        {
            try
            {
                block();
            }
            catch (invalid_credential_error& ex)
            {
                critial_message_box(gen_message_from_exception("用户名或密码无效：", ex), "登录失败");
            }
            catch (duplication_error& ex)
            {
                critial_message_box(gen_message_from_exception("用户名已被占用：", ex), "注册失败");
            }
        });
    
}
}
