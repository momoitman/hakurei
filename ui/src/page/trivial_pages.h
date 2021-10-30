#pragma once

#include "model/user.h"
#include <DFrame>
#include <DDialog>
#include <DWarningButton>
#include <DSuggestButton>
#include <DLineEdit>
#include <DLabel>
#include <string>

DWIDGET_USE_NAMESPACE

namespace hakurei::ui
{
class deposit_page : public DDialog
{
    Q_OBJECT
public:
    explicit deposit_page(QWidget* parent);
    ~deposit_page() override = default;

public slots:
    void reset_and_show();
signals:
    void on_request_deposit(int money_cents);

private slots:
    void on_deposit_click();

private:
    DLineEdit* _amount_edit;
    DSuggestButton* _ok_button;
};

class user_page : public DDialog
{
    Q_OBJECT
public:
    explicit user_page(QWidget* parent);
    ~user_page() override = default;

public slots:
    void update(core::model::user const& user, bool show_balance, bool delete_enabled);
signals:
    void on_delete_user(std::string_view user_id);

private slots:
    void on_delete_user_click();

private:
    DLabel *_id_label, *_name_label, *_balance_label, *_contact_label, *_address_label;
    DWarningButton* _delete_button;

    std::string _user_id;
};
}