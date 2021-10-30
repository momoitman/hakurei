#pragma once

#include "model/item.h"
#include <DFrame>
#include <DDialog>
#include <DWarningButton>
#include <DSuggestButton>
#include <DLabel>
#include <string>

DWIDGET_USE_NAMESPACE

namespace hakurei::ui
{
/*
 * These pages should be made like those
 * connection edit pages in DTK Control Center
 * that is displayed on the right of the main
 * window, leaving the left to be blank (semi-transparent)
 * But I just don't have so much time
 * So i made them simply dialogs.
 */
class item_customer_page : public DDialog
{
    Q_OBJECT
public:
    explicit item_customer_page(QWidget* parent);
    ~item_customer_page() override = default;

public slots:
    void update(core::model::item const& item, bool purchase_enabled, bool delete_enabled);
signals:
    void on_purchase_item(std::string item_id);
    void on_delete_item(std::string item_id);
private slots:
    void on_purchase_click();

private:
    bool _purchase_enabled, _delete_enabled;
    std::string _item_id;

    DLabel *_name_label, *_price_label, *_desc_label;
    DSuggestButton* _purchase_button;
    DWarningButton* _delete_button;
};

class item_seller_page : public DFrame
{
    Q_OBJECT
public:
    explicit item_seller_page(QWidget* parent);
    ~item_seller_page() override = default;

public slots:
    void update(std::string item_id, bool delete_enabled, bool editing_enabled);
signals:
    void on_edit_item(std::string item_id);
    void on_delete_item(std::string item_id);

private:
    bool _delete_enabled, _editing_enabled;
    std::string _item_id;
};
}  // namespace hakurei::ui