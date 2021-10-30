#pragma once

#include <model/user.h>

#include <QLineEdit>

#include <DApplication>
#include <DMainWindow>
#include <DDialog>
#include <DWidgetUtil>
#include <DLabel>
#include <DTitlebar>
#include <DProgressBar>
#include <DPushButton>
#include <DSuggestButton>
#include <DLineEdit>

DWIDGET_USE_NAMESPACE

namespace hakurei::ui
{
class register_or_edit_window : public DDialog
{
    Q_OBJECT
public:
    explicit register_or_edit_window(QWidget* parent);
    virtual ~register_or_edit_window() override = default;

public slots:
    void reset_and_show();
    void reset_and_show(core::model::user const& u);
    void on_register_clicked();
signals:
    void on_register(
        QString const& username, QString const& password,
        QString const& contact, QString const& address);

private:
    DLabel* _head_label;
    DLineEdit* _username_text;
    DLineEdit* _password_text;
    DLineEdit* _contact_text;
    DLineEdit* _address_text;
    DSuggestButton* _ok_btn;
    bool _password_noneed;
};
}  // namespace hakurei::ui
