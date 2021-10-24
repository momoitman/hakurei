#include "main_toolbar.h"

namespace hakurei::ui
{
main_toolbar::main_toolbar(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    _switcher_btns = new DButtonBox(this);
    _switcher_btns->setFixedWidth(240);

    _customer_btn = new DButtonBoxButton(tr("买家"), _switcher_btns);
    _seller_btn = new DButtonBoxButton(tr("卖家"), _switcher_btns);
    _admin_btn = new DButtonBoxButton(tr("管理员"), _switcher_btns);

    _customer_btn->setCheckable(true);
    _customer_btn->setChecked(true);
    _seller_btn->setCheckable(true);
    _admin_btn->setCheckable(true);
    _admin_btn->setEnabled(false);
    _admin_btn->setVisible(false);
    _customer_btn->setFocusPolicy(Qt::TabFocus);
    _seller_btn->setFocusPolicy(Qt::TabFocus);
    _admin_btn->setFocusPolicy(Qt::TabFocus);

    QList<DButtonBoxButton*> list;
    list << _customer_btn << _seller_btn << _admin_btn;
    _switcher_btns->setButtonList(list, true);

    setFocusProxy(_customer_btn);
    layout->addWidget(_switcher_btns, 0, Qt::AlignLeft);
    layout->addStretch();

    _status_lbl = new DLabel(this);
    layout->addWidget(_status_lbl, 0, Qt::AlignRight);
    layout->addSpacing(30);

    connect(_customer_btn, &DButtonBoxButton::toggled, this, [=](bool) { emit on_tab_customer_clicked(); });
    connect(_seller_btn, &DButtonBoxButton::toggled, this, [=](bool) { emit on_tab_seller_clicked(); });
    connect(_admin_btn, &DButtonBoxButton::toggled, this, [=](bool) { emit on_tab_admin_clicked(); });
}

void main_toolbar::set_enabled(bool enabled, bool admin_enabled)
{
    if (!enabled)
    {
        _customer_btn->setChecked(false);
        _seller_btn->setChecked(false);
        _admin_btn->setChecked(false);
        _seller_btn->setEnabled(false);
        _customer_btn->setEnabled(false);
        _admin_btn->setEnabled(false);
        return;
    }

    _customer_btn->setChecked(true);
    _seller_btn->setChecked(false);
    _admin_btn->setChecked(false);
    _seller_btn->setEnabled(true);
    _customer_btn->setEnabled(true);
    _admin_btn->setEnabled(admin_enabled);
    _admin_btn->setVisible(admin_enabled);

    QList<DButtonBoxButton*> list;
    list << _customer_btn << _seller_btn;
    if (admin_enabled)
        list << _admin_btn;
    _switcher_btns->setButtonList(list, true);
}

void main_toolbar::set_username(QString const& username)
{
    _status = tr("当前登录为：") + username;
    _status_lbl->setText(_status);
}
}
