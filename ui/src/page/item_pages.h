#pragma once

#include "model/item.h"
#include <DLineEdit>
#include <DFrame>
#include <DDialog>
#include <DWarningButton>
#include <DPlainTextEdit>
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
class item_show_page : public DDialog
{
    Q_OBJECT
public:
    explicit item_show_page(QWidget* parent);
    ~item_show_page() override = default;

public slots:
    void update(core::model::item const& item, bool purchase_visible, bool purchase_enabled, bool delete_enabled);
signals:
    void on_purchase_item(std::string item_id);
    void on_delete_item(std::string item_id);
private slots:
    void on_purchase_click();
    void on_delete_click();

private:
    std::string _item_id;

    DLabel *_id_label, *_name_label, *_price_label, *_desc_label;
    DSuggestButton* _purchase_button;
    DWarningButton* _delete_button;
};

class item_edit_page : public DDialog
{
    Q_OBJECT
public:
    explicit item_edit_page(QWidget* parent);
    ~item_edit_page() override = default;

public slots:
    void update_new();
    void update(core::model::item const& item, bool delete_enabled);
signals:
    void on_create_item(QString const& name, int price_cents, QString const& desc);
    void on_edit_item(std::string_view item_id, QString const& name, int price_cents, QString const& desc);
    void on_delete_item(std::string_view item_id);

private slots:
    void on_save_click();
    void on_delete_click();

private:
    bool _creating_item;
    std::string _item_id;
    DLabel* _id_label;
    DLineEdit *_name_edit, *_price_edit;
    DPlainTextEdit* _desc_edit;
    DSuggestButton* _save_button;
    DWarningButton* _delete_button;
};
}  // namespace hakurei::ui