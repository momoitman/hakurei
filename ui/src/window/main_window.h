#pragma once

#include <DApplication>
#include <DMainWindow>
#include <DDialog>
#include <DWidgetUtil>
#include <DApplicationSettings>
#include <DTitlebar>
#include <DProgressBar>
#include <DShadowLine>
#include <DFontSizeManager>
#include <DStackedWidget>

#include "main_toolbar.h"
#include "login_window.h"

DWIDGET_USE_NAMESPACE

namespace hakurei::ui
{

class main_window : public DMainWindow
{
    Q_OBJECT
public:
    explicit main_window();
    ~main_window() override {}
public slots:
    void on_login(QString username, QString password);
    void on_logout();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    main_toolbar* _toolbar;
    DShadowLine* _title_line;
    DStackedWidget* _pages;

    login_window* _login_window;
};

}