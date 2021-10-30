#pragma once

#include "widget/info_bar_widget.h"
#include "service/services.h"
#include "model/table_models.h"
#include "page/item_pages.h"
#include "page/trivial_pages.h"

#include <DListView>
#include <DWidget>
#include <DStackedWidget>
#include <DFrame>
#include <DSuggestButton>
#include <QTableView>

DWIDGET_USE_NAMESPACE;

namespace hakurei::ui
{
class seller_page : public DFrame
{
    Q_OBJECT
public:
    explicit seller_page(QWidget* parent);
    ~seller_page() override = default;

public slots:
    void update_injector(core::service::service_injector* ij);
    void update(core::service::auth_token token);

private slots:
    void on_add_item_click();
    void on_item_table_double_click(QModelIndex const& index);
    void on_order_table_double_click(QModelIndex const& index);
    void show_item_by_id(std::string_view item);
    void show_item(model::item* item);
    void show_edit_item(model::item const* item);
    void show_user_by_id(std::string user_id);
    void create_item(QString const& name, int price_cents, QString const& desc);
    void edit_item(std::string_view item_id, QString const& name, int price_cents, QString const& desc);
    void delete_item(std::string_view item_id);

private:
    core::service::service_injector* _ij = nullptr;
    core::service::auth_service* _auth_svc = nullptr;
    core::service::item_service* _item_svc = nullptr;
    core::service::order_service* _order_svc = nullptr;
    core::service::auth_token _token = 0;

    user_page* _user_page;
    item_show_page* _item_show_page;
    item_edit_page* _item_edit_page;

    info_bar_widget* _info_bar;
    DSuggestButton* _add_item_button;

    QTableView* _my_item_table;
    QTableView* _my_order_table;
    model::item_table_model* _item_table_model;
    model::order_table_model* _order_table_model;
    std::vector<core::model::item> _my_items;
    std::vector<core::model::order> _my_orders;
};
}