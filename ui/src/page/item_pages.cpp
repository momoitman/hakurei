#include "item_pages.h"

#include "util/str_util.h"
#include <QScrollArea>
#include <QBoxLayout>
#include <QMessageBox>

namespace hakurei::ui
{
item_show_page::item_show_page(QWidget* parent)
    : DDialog(parent)
{
    setModal(true);
    setMinimumWidth(400);
    resize(400, 600);
    setContentsMargins(20, 20, 20, 20);
    hide();

    auto vlayout = new QVBoxLayout(this);
    vlayout->setSpacing(5);

    _purchase_button = new DSuggestButton(tr("购买"), this);
    _delete_button = new DWarningButton(this);
    _delete_button->setText(tr("删除"));
    vlayout->addWidget(_purchase_button);
    vlayout->addWidget(_delete_button);

    _id_label = new DLabel(this);
    _name_label = new DLabel(this);
    _price_label = new DLabel(this);
    _desc_label = new DLabel(this);

    _id_label->setStyleSheet("font: 10px");
    _name_label->setStyleSheet("font: 28px");
    _price_label->setStyleSheet("font: 23px");
    _desc_label->setStyleSheet("font: 12px");

    _price_label->setAlignment(Qt::AlignRight);
    _name_label->setWordWrap(true);
    _desc_label->setWordWrap(true);
    _desc_label->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    vlayout->addWidget(_id_label);
    vlayout->addWidget(_name_label);
    vlayout->addWidget(_price_label);

    auto scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _desc_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    scroll->setWidget(_desc_label);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    vlayout->addWidget(scroll);

    // code specified for DDialog
    QWidget* mainContent = new QWidget(this);
    mainContent->setLayout(vlayout);
    addContent(mainContent);

    connect(_purchase_button, &DSuggestButton::clicked, this, &item_show_page::on_purchase_click);
    connect(_delete_button, &DSuggestButton::clicked, [&](){ emit on_delete_item(_item_id); });
}

void item_show_page::update(core::model::item const& item, bool purchase_visible, bool purchase_enabled, bool delete_enabled)
{
    _item_id = item.id();
    _id_label->setText(QString::fromStdString(_item_id));
    _name_label->setText(QString::fromStdString(item.name()));
    _price_label->setText(arg_price_cents(tr("￥%1"), item.price_cents()));
    _desc_label->setText(
        tr("上架于 %1\n%2")
            .arg(to_string_datetime(item.on_stock_time()))
            .arg(QString::fromStdString(item.description()))
    );

    _purchase_button->setVisible(purchase_visible);
    _purchase_button->setEnabled(purchase_enabled);
    _delete_button->setVisible(delete_enabled);
    _delete_button->setEnabled(delete_enabled);
}

void item_show_page::on_purchase_click()
{
    auto conf = QMessageBox::question(
        this,
        "购买", tr("确定以 %1 购买商品 %2 吗？").arg(_price_label->text()).arg(_name_label->text()),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (conf != QMessageBox::Yes)
        return;
    emit on_purchase_item(_item_id);
}

item_seller_page::item_seller_page(QWidget* parent)
{
}

void item_seller_page::update(std::string item_id, bool delete_enabled, bool editing_enabled)
{
}
}
