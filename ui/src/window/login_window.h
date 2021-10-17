#pragma once

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
    void resetAndShow();

protected:
    void closeEvent(QCloseEvent* event) override;
private:
    DLineEdit* _username_text;
    DLineEdit* _password_text;
    DPushButton* _register_btn;
    DSuggestButton* _login_btn;
};
}

