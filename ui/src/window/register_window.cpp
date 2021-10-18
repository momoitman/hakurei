#include "register_window.h"

#include <DLabel>

namespace hakurei::ui
{
register_window::register_window(QWidget* parent): DDialog(parent)
{
    setModal(true);
    auto layout = new QVBoxLayout;

    auto head_label = new DLabel(tr("注册 Hakurei"));
    head_label->setStyleSheet("font: 18pt;");

    _username_text = new DLineEdit;
    _password_text = new DLineEdit;
    _contact_text = new DLineEdit;
    _address_text = new DLineEdit;

    _username_text->setPlaceholderText(tr("用户名"));
    _password_text->setPlaceholderText(tr("密码"));
    _contact_text->setPlaceholderText(tr("联系方式"));
    _address_text->setPlaceholderText(tr("地址"));

    _ok_btn = new DSuggestButton(tr("注册"));

    _password_text->setEchoMode(QLineEdit::EchoMode::Password);
    
    layout->setSpacing(40);
    layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layout->addWidget(head_label);
    layout->addWidget(_username_text);
    layout->addWidget(_password_text);
    layout->addWidget(_contact_text);
    layout->addWidget(_address_text);
    layout->addWidget(_ok_btn);
    layout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    resize(660, 470);
    setFixedSize(660, 470);

    QWidget* mainContent = new QWidget;
    mainContent->setLayout(layout);
    addContent(mainContent);

    connect(_ok_btn, &DSuggestButton::clicked, this, &register_window::on_register_clicked);

    hide();
}

void register_window::reset_and_show()
{
    _username_text->clear();
    _password_text->clear();
    _contact_text->clear();
    _address_text->clear();

    show();
    
    setFocus();
}

void register_window::on_register_clicked()
{
    // TODO String sanitize!
    emit on_register(_username_text->text(), _password_text->text(), _contact_text->text(), _address_text->text());
}
}
