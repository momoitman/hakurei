#pragma once

#include <DListView>
#include <DWidget>
#include <DLabel>
#include <DFrame>
#include <service/services.h>

DWIDGET_USE_NAMESPACE;

namespace hakurei::ui
{
class info_bar_widget : public DFrame
{
    Q_OBJECT

public:
    explicit info_bar_widget(QWidget* parent);
    ~info_bar_widget() override = default;

public slots:
    void updateUsername(QString const& username);
    void updateContent(QString const& content);

protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    DLabel* header_label;
    DLabel* content_label;
};

}