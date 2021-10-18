#pragma once

#include <QLineEdit>

#include <DApplication>
#include <DMainWindow>
#include <DDialog>
#include <DWidgetUtil>
#include <DApplicationSettings>
#include <DTitlebar>
#include <DProgressBar>
#include <DPushButton>
#include <DSuggestButton>
#include <DLineEdit>

DWIDGET_USE_NAMESPACE

namespace hakurei::ui
{
class register_window : public DDialog
{
    Q_OBJECT
public:
    explicit register_window(QWidget* parent);
    virtual ~register_window() override = default;

public slots:
    void reset_and_show();
    void on_register_clicked();
signals:
    void on_register(
        QString const& username, QString const& password,
        QString const& contact, QString const& address
    );
    
private:
    DLineEdit* _username_text;
    DLineEdit* _password_text;
    DLineEdit* _contact_text;
    DLineEdit* _address_text;
    DSuggestButton* _ok_btn;
};
}  // namespace hakurei::ui
