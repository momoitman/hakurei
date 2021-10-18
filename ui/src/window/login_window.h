#pragma once

#include "register_window.h"

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
class main_window;

class login_window : public DDialog
{
    Q_OBJECT
public:
    explicit login_window(main_window* parent);
    virtual ~login_window() override = default;

public slots:
    void reset_and_show();
private slots:  
    void on_register_click();
    void on_login_click();

signals:
    void on_login(QString const& username, QString const& password);
    void on_register(
        QString const& username, QString const& password,
        QString const& contact, QString const& address);

protected:
    void closeEvent(QCloseEvent* event) override;
    void hideEvent(QHideEvent* event) override;
private:
    DLineEdit* _username_text;
    DLineEdit* _password_text;
    DPushButton* _register_btn;
    DSuggestButton* _login_btn;

    register_window* _register_window;
};
}

