#include "version.h"

#include "window/main_window.h"

#include <DApplication>
#include <DWidgetUtil>
#include <DApplicationSettings>
DWIDGET_USE_NAMESPACE

using namespace hakurei::ui;

int main(int argc, char* argv[])
{
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    DApplication hakurei_app(argc, argv);
    hakurei_app.setOrganizationName(HAKUREI_ORG);
    hakurei_app.setApplicationName(HAKUREI_APP_NAME);
    hakurei_app.setApplicationVersion(HAKUREI_VERSION);
    hakurei_app.setProductIcon(QIcon(":images/hakurei.png"));
    hakurei_app.setProductName(HAKUREI_APP_NAME);
    hakurei_app.setApplicationDescription(DApplication::tr("An used-item C2C platform."));

    hakurei_app.loadTranslator();
    hakurei_app.setApplicationDisplayName("Hakurei");

    // 保存程序的窗口主题设置
    DApplicationSettings as;
    Q_UNUSED(as)

    main_window mw;
    mw.show();
    Dtk::Widget::moveToCenter(&mw);

    return hakurei_app.exec();
}