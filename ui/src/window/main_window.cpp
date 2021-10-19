#include "main_window.h"

#include "common_exception_handle.h"

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

    resize(800, 600);
    titlebar()->setIcon(QIcon::fromTheme(":images/hakurei.png"));
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

    _pages = new DStackedWidget(this);
    setCentralWidget(_pages);
    setContentsMargins(0, 0, 0, 0);

    _title_line = new DShadowLine(_pages);
    _title_line->setFixedWidth(width());
    _title_line->setFixedHeight(10);
    _title_line->move(0, 0);
    _title_line->show();

    _login_window = new login_window(nullptr);
    _login_window->reset_and_show();
    connect(_login_window, &login_window::on_login, this, &main_window::on_login);
    connect(_login_window, &login_window::on_register, this, &main_window::on_register);

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

void main_window::resizeEvent(QResizeEvent* event)
{
    DMainWindow::resizeEvent(event);
    _title_line->setFixedWidth(width());
}

void main_window::refresh_after_token_changed()
{
    _login_window->hide();
    _toolbar->set_enabled(true, _auth_svc->is_user_admin(_auth_token));
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
