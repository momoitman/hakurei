#include "info_bar_widget.h"

#include <DLabel>

namespace hakurei::ui
{
info_bar_widget::info_bar_widget(QWidget* parent)
{
    setFixedHeight(130);
    setFrameRounded(true);
    setLineWidth(15);

    auto icon_label = new DLabel(this);
    icon_label->setGeometry(25, 25, 80, 80);
    icon_label->setPixmap(QPixmap(":images/user_icon.png").scaled(80, 80, Qt::KeepAspectRatio));

    header_label = new DLabel(this);
    content_label = new DLabel(this);
    header_label->move(130, 25);
    content_label->move(130, 68);

    header_label->setStyleSheet("font: 36px;");
    content_label->setStyleSheet("font: 18px;");
}

void info_bar_widget::updateUsername(QString const& username)
{
    header_label->setText(tr("Howdy, ") + username);
}

void info_bar_widget::updateContent(QString const& content)
{
    content_label->setText(content);
}
}
