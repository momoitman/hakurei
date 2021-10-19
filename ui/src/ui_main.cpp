#include "version.h"

#include "window/main_window.h"

#include <DApplication>
#include <DWidgetUtil>
#include <DApplicationSettings>
DWIDGET_USE_NAMESPACE

using namespace hakurei::ui;

#include <QVariant>
#include <QApplication>
#include <DAboutDialog>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QVBoxLayout>

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
    Dtk::Widget::moveToCenter(&mw);

    auto about_dlg = new DAboutDialog(&mw);
    hakurei_app.setAboutDialog(about_dlg);
    about_dlg->setProductName(HAKUREI_APP_NAME);
    about_dlg->setProductIcon(QIcon(":images/hakurei.png"));
    about_dlg->setVersion(HAKUREI_VERSION);
    about_dlg->setDescription(DApplication::tr("An used-item C2C platform authored by Jiang Zhirui."));
    about_dlg->setCompanyLogo(QPixmap(":images/nju.png"));
    about_dlg->setWebsiteName("Nanjing University");
    about_dlg->setWebsiteLink("https://www.nju.edu.cn/");
    about_dlg->setAcknowledgementVisible(false);
    about_dlg->setLicense("Licensed under GNU Lesser General Public License v3");

    return hakurei_app.exec();
}