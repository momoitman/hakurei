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
class register_window : public DDialog
{
    Q_OBJECT
public:
    explicit register_window(QWidget* parent);
    virtual ~register_window() override = default;

public slots:
    void resetAndShow();

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    DLineEdit* _username_text;
    DLineEdit* _password_text;
    DLineEdit* _contact_text;
    DLineEdit* _address_text;
    DPushButton* _cancel_btn;
    DSuggestButton* _ok_btn;
};
}  // namespace hakurei::ui
