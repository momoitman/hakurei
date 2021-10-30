#include "login_window.h"

#include <QScrollArea>
#include <QPicture>
#include <DLabel>

namespace hakurei::ui
{
login_window::login_window(main_window* parent)
    : DDialog(reinterpret_cast<QWidget*>(parent))
{
    setModal(true);
    setVisible(false);
    //setTitle(tr("登录 Hakurei"));
    setFixedSize(660, 355);

    //QPalette pal;
    //pal.setColor(QPalette::Window, Qt::lightGray);
    //setAutoFillBackground(true);
    //setPalette(pal);

    _username_text = new DLineEdit(this);
    _password_text = new DLineEdit(this);

    _username_text->setPlaceholderText(tr("用户名"));
    _password_text->setPlaceholderText(tr("密码"));

    _password_text->setEchoMode(QLineEdit::EchoMode::Password);

    _login_btn = new DSuggestButton(tr("登录"), this);
    _register_btn = new DPushButton(tr("注册"), this);

    auto head_label = new DLabel(tr("登录 Hakurei"), this);
    head_label->setStyleSheet("font: 18pt;");
    auto icon_label = new DLabel("hello", this);
    icon_label->setPixmap(QPixmap(":images/reimu_tachie.png").scaled(200, 315, Qt::KeepAspectRatio));

    _username_text->move(230, 93);
    _password_text->move(230, 160);
    _register_btn->move(230, 240);
    _login_btn->move(447, 240);
    head_label->move(230, 40);
    icon_label->move(0, 20);

    _username_text->resize(400, 33);
    _password_text->resize(400, 33);
    _register_btn->resize(187, 33);
    _login_btn->resize(187, 33);
    head_label->adjustSize();
    icon_label->resize(200, 315);

    _register_window = new register_or_edit_window(this);

    connect(_register_btn, &DPushButton::clicked, this, &login_window::on_register_click);
    connect(_login_btn, &DPushButton::clicked, this, &login_window::on_login_click);
    connect(_username_text, &DLineEdit::returnPressed, this, &login_window::on_login_click);
    connect(_password_text, &DLineEdit::returnPressed, this, &login_window::on_login_click);
    connect(_register_window, &register_or_edit_window::on_register, this, &login_window::on_register);
}

void login_window::reset_and_show()
{
    _username_text->clear();
    _password_text->clear();
    show();
    setFocus();
    _username_text->setText("test_admin");
    _password_text->setText("123456abcd");
    _username_text->setFocus();
}

void login_window::on_register_click()
{
    emit _register_window->reset_and_show();
}

void login_window::on_login_click()
{
    emit on_login(_username_text->text(), _password_text->text());
}

void login_window::closeEvent(QCloseEvent* event)
{
    DDialog::closeEvent(event);
    QCoreApplication::quit();
}

void login_window::hideEvent(QHideEvent* event)
{
    DDialog::hideEvent(event);
    _register_window->hide();
}
}
