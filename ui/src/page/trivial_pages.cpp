#include "trivial_pages.h"

#include <limits>
#include <cmath>
#include <QBoxLayout>
#include <QValidator>

namespace hakurei::ui
{
deposit_page::deposit_page(QWidget* parent)
{
    setModal(true);
    setFixedSize(400, 200);
    setContentsMargins(20, 20, 20, 20);
    hide();

    auto vlayout = new QVBoxLayout(this);
    vlayout->setSpacing(5);

    _amount_edit = new DLineEdit(this);
    _amount_edit->setPlaceholderText(tr("充值金额"));
    _amount_edit->lineEdit()->setValidator(
        new QDoubleValidator(0, std::numeric_limits<double>::max(), 2, this));
    vlayout->addWidget(_amount_edit);

    _ok_button = new DSuggestButton(tr("充值"), this);
    vlayout->addWidget(_ok_button);

    QWidget* mainContent = new QWidget;
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
}
