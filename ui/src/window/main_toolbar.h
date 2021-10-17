#pragma once

#include <DButtonBox>
#include <DSearchEdit>
#include <DWidget>

DWIDGET_USE_NAMESPACE

namespace hakurei::ui
{
class main_toolbar : public DWidget
{
    Q_OBJECT

public:
    explicit main_toolbar(QWidget* parent);
    ~main_toolbar() override = default;
public slots:
    void set_enabled(bool enabled, bool admin_enabled);
signals:
    void on_tab_customer_clicked();
    void on_tab_seller_clicked();
    void on_tab_admin_clicked();

private:
    DButtonBox* _switcher_btns;
    DButtonBoxButton* _customer_btn;
    DButtonBoxButton* _seller_btn;
    DButtonBoxButton* _admin_btn;
};
}