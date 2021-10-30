#include "trivial_pages.h"

#include <limits>
#include <cmath>
#include <DMessageBox>
#include <QBoxLayout>
#include <QValidator>

#include "util/str_util.h"

namespace hakurei::ui
{
deposit_page::deposit_page(QWidget* parent)
    : DDialog(parent)
{
    setModal(true);
    setFixedSize(400, 200);
    setContentsMargins(20, 20, 20, 20);
    hide();

    auto vlayout = new QVBoxLayout;
    vlayout->setSpacing(5);

    _amount_edit = new DLineEdit(this);
    _amount_edit->setPlaceholderText(tr("充值金额"));
    _amount_edit->lineEdit()->setValidator(
        new QDoubleValidator(0, std::numeric_limits<double>::max(), 2, this));
    vlayout->addWidget(_amount_edit);

    _ok_button = new DSuggestButton(tr("充值"), this);
    vlayout->addWidget(_ok_button);

    QWidget* mainContent = new QWidget(this);
    mainContent->setLayout(vlayout);
    addContent(mainContent);

    connect(_ok_button, &DSuggestButton::clicked, this, &deposit_page::on_deposit_click);
    connect(_amount_edit, &DLineEdit::returnPressed, this, &deposit_page::on_deposit_click);
}

void deposit_page::reset_and_show()
{
    _amount_edit->clear();
    show();
    _amount_edit->setFocus();
}

void deposit_page::on_deposit_click()
{
    int x = static_cast<int>(std::round(_amount_edit->text().toDouble() * 100.0));
    if (x > 0)
        emit on_request_deposit(x);
}

user_page::user_page(QWidget* parent):DDialog(parent)
{
    setModal(true);
    setFixedSize(450, 450);
    setContentsMargins(20, 20, 20, 20);
    hide();

    auto mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(5);

    _delete_button = new DWarningButton(this);
    _delete_button->setText(tr("删除用户"));
    mainLayout->addWidget(_delete_button);
    
    _id_label = new DLabel(this);
    _name_label = new DLabel(this);
    _balance_label = new DLabel(this);

    _id_label->setStyleSheet("font: 10px");
    _name_label->setStyleSheet("font: 28px");
    _balance_label->setStyleSheet("font: 23px");

    mainLayout->addWidget(_id_label);
    mainLayout->addWidget(_name_label);
    mainLayout->addWidget(_balance_label);

    QWidget* detail_w = new QWidget(this);
    _contact_label = new DLabel(detail_w);
    _address_label = new DLabel(detail_w);
    auto gLayout = new QGridLayout(detail_w);
    gLayout->setSpacing(5);

    auto contact_hint_label = new DLabel(tr("联系方式"), detail_w);
    auto address_hint_label = new DLabel(tr("地址"), detail_w);

    _contact_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _address_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    contact_hint_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    address_hint_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    _contact_label->setStyleSheet("font: 12px");
    _address_label->setStyleSheet("font: 12px");

    gLayout->addWidget(contact_hint_label, 0, 0);
    gLayout->addWidget(address_hint_label, 1, 0);
    gLayout->addWidget(_contact_label, 0, 1);
    gLayout->addWidget(_address_label, 1, 1);

    mainLayout->addWidget(detail_w);

    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(spacer);

    // code specified for DDialog
    QWidget* mainContent = new QWidget(this);
    mainContent->setLayout(mainLayout);
    addContent(mainContent);
}

void user_page::update(core::model::user const& user, bool show_balance, bool delete_enabled)
{
    _user_id = user.id();
    _id_label->setText(QString::fromStdString(_user_id));
    _name_label->setText(QString::fromStdString(user.name()));
    if (show_balance)
    {
        _balance_label->setVisible(true);
        _balance_label->setText(arg_price_cents(tr("￥%1"), user.balance_cents()));
    }
    else
        _balance_label->setVisible(false);
    _contact_label->setText(QString::fromStdString(user.contact()));
    _address_label->setText(QString::fromStdString(user.address()));

    _delete_button->setEnabled(delete_enabled);
    _delete_button->setVisible(delete_enabled);
}

void user_page::on_delete_user_click()
{
    auto conf = DMessageBox::question(
        this,
        "购买", tr("确定删除用户 %1(%2) 吗？").arg(_name_label->text()).arg(_id_label->text()),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (conf != QMessageBox::Yes)
        return;
    emit on_delete_user(_user_id);
}
}
