#pragma once

#include "model/item.h"
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
}