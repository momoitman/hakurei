#include "register_or_edit_window.h"

#include "common_exception_handle.h"
#include <DLabel>

namespace hakurei::ui
{
register_or_edit_window::register_or_edit_window(QWidget* parent)
    : DDialog(parent)
{
    setModal(true);
    setVisible(false);
    //setTitle(tr("注册 Hakurei"));
    auto layout = new QVBoxLayout;

    _head_label = new DLabel(this);
    _head_label->setStyleSheet("font: 18pt;");

    _username_text = new DLineEdit(this);
    _password_text = new DLineEdit(this);
    _contact_text = new DLineEdit(this);
    _address_text = new DLineEdit(this);

    _username_text->setPlaceholderText(tr("用户名"));
    _password_text->setPlaceholderText(tr("密码"));
    _contact_text->setPlaceholderText(tr("联系方式"));
    _address_text->setPlaceholderText(tr("地址"));

    _ok_btn = new DSuggestButton(tr("注册"), this);
    _ok_btn->setAutoDefault(true);
    _ok_btn->setDefault(true);

    _password_text->setEchoMode(QLineEdit::EchoMode::Password);
    
    layout->setSpacing(40);
    layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addWidget(_head_label);
    layout->addWidget(_username_text);
    layout->addWidget(_password_text);
    layout->addWidget(_contact_text);
    layout->addWidget(_address_text);
    layout->addWidget(_ok_btn);
    layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    resize(660, 470);
    setFixedSize(660, 470);

    QWidget* mainContent = new QWidget(this);
    mainContent->setLayout(layout);
    addContent(mainContent);

    connect(_ok_btn, &DSuggestButton::clicked, this, &register_or_edit_window::on_register_clicked);

    hide();
}

void register_or_edit_window::reset_and_show()
{
    _head_label->setText(tr("注册 Hakurei"));
    _username_text->clear();
    _password_text->clear();
    _contact_text->clear();
    _address_text->clear();

    show();
    
    setFocus();
    _password_noneed = false;
}

void register_or_edit_window::reset_and_show(core::model::user const& u)
{
    _head_label->setText(tr("修改用户信息"));
    _password_text->setPlaceholderText(tr("密码(留空则不变)"));
    _username_text->setText(QString::fromStdString(u.name()));
    _password_text->clear();
    _contact_text->setText(QString::fromStdString(u.contact()));
    _address_text->setText(QString::fromStdString(u.address()));

    show();

    setFocus();
    _password_noneed = true;
}

void register_or_edit_window::on_register_clicked()
{
    if (_username_text->text().isEmpty()
        || _contact_text->text().isEmpty() || _address_text->text().isEmpty())
        critial_message_box("所有项均为必填。", "验证失败");
    else if ((!_password_noneed || !_password_text->text().isEmpty()) && _password_text->text().length() < 6)
        critial_message_box("弱密码：密码至少需 6 位。", "验证失败");
    else
        emit on_register(_username_text->text(), _password_text->text(), 
            _contact_text->text(), _address_text->text());
}
}
