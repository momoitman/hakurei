#include "main_window.h"

namespace hakurei::ui
{
main_window::main_window()
{
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
    _login_window->resetAndShow();
    hide();
}

void main_window::on_login(QString username, QString password)
{
    
}

void main_window::on_logout()
{
    // TODO core: logout
    _toolbar->set_enabled(false, false);
    hide();
    _login_window->resetAndShow();
}

void main_window::resizeEvent(QResizeEvent* event)
{
    DMainWindow::resizeEvent(event);
    _title_line->setFixedWidth(width());
}
}
