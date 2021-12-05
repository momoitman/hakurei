#pragma once

#include "service/services.h"
#include "model/user.h"
#include "model/table_models.h"

#include <DListView>
#include <DWidget>
#include <DStackedWidget>
#include <DFrame>

DWIDGET_USE_NAMESPACE;

namespace hakurei::ui
{
namespace admin_subpages
{
template <class T, class This, bool remove_enabled>
class abstract_subpage;
class user_subpage;
class item_subpage;
class order_subpage;
}

class admin_page : public DFrame {
    Q_OBJECT

public:
    explicit admin_page(QWidget* parent);
    ~admin_page() override = default;

public slots:
    void update(core::service::auth_token token);
    void update_injector(core::service::service_injector* ij);

private slots:
    void on_switch_subpage(QModelIndex const& qidx);

private:
    DListView* _page_selector;
    DStackedWidget* _pages;

    admin_subpages::user_subpage* _user_page;
    admin_subpages::item_subpage* _item_page;
    admin_subpages::order_subpage* _order_page;
    static constexpr int _user_page_idx = 0, _item_page_idx = 1, _order_page_idx = 2;

    core::service::service_injector* _ij = nullptr;
    core::service::auth_service* _auth_svc = nullptr;
    core::service::item_service* _item_svc = nullptr;
    core::service::order_service* _order_svc = nullptr;
    core::service::auth_token _token = 0;

    template <typename>
    friend class admin_subpages::abstract_subpage;
    friend class admin_subpages::user_subpage;
    friend class admin_subpages::item_subpage;
    friend class admin_subpages::order_subpage;
};

namespace admin_subpages
{
template <class T> struct model_type_t { using type = std::false_type; };
template <> struct model_type_t<model::user> { using type = model::user_table_model; };
template <> struct model_type_t<model::item> { using type = model::item_table_model; };
template <> struct model_type_t<model::order> { using type = model::order_table_model; };

template<class T, class This, bool remove_enabled>
class abstract_subpage
{
public:
    using model_type = typename model_type_t<T>::type;
    explicit abstract_subpage(admin_page* parent, This* t);
    virtual ~abstract_subpage() = default;

public:
    void update_page();
    
protected:
    virtual void update_items() = 0;
    virtual void remove_item(std::string_view id) = 0;

    This* _t;
    admin_page* _pg;

    QTableView* _table_view;
    model_type* _model;
    QMenu* _remove;

    std::vector<T> _items;
};

class user_subpage : public DFrame, public abstract_subpage<model::user, user_subpage, true>
{
    Q_OBJECT
public:
    explicit user_subpage(admin_page* parent);
    ~user_subpage() override = default;

public:
    void update_items() override;
    void remove_item(std::string_view id) override;
};

class item_subpage : public DFrame, public abstract_subpage<model::item, item_subpage, true>
{
    Q_OBJECT
public:
    explicit item_subpage(admin_page* parent);
    ~item_subpage() override = default;
    
public:
    void update_items() override;
    void remove_item(std::string_view id) override;
};

class order_subpage : public DFrame, public abstract_subpage<model::order, order_subpage, false>
{
    Q_OBJECT
public:
    explicit order_subpage(admin_page* parent);
    ~order_subpage() override = default;

public:
    void update_items() override;
    void remove_item(std::string_view id) override {};
};
}
}