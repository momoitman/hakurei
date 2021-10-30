#include "item_pages.h"

#include "util/str_util.h"
#include "common_exception_handle.h"

#include <QScrollArea>
#include <QBoxLayout>
#include <QMessageBox>
#include <cmath>

namespace hakurei::ui
{
item_show_page::item_show_page(QWidget* parent)
    : DDialog(parent)
{
    setModal(true);
    setMinimumWidth(400);
    resize(400, 600);
    setContentsMargins(20, 0, 20, 20);
    hide();

    auto vlayout = new QVBoxLayout(this);
    vlayout->setSpacing(0);

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
    connect(_delete_button, &DSuggestButton::clicked, this, &item_show_page::on_delete_click);
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

void item_show_page::on_delete_click()
{
    auto conf = QMessageBox::question(
        this,
        "删除", tr("确定删除商品 %1(%2) 吗？").arg(_name_label->text()).arg(_id_label->text()),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (conf != QMessageBox::Yes)
        return;
    emit on_delete_item(_item_id);
}

item_edit_page::item_edit_page(QWidget* parent)
    : DDialog(parent)
{
    setModal(true);
    setMinimumWidth(400);
    resize(400, 600);
    setContentsMargins(20, 0, 20, 20);
    hide();

    auto vlayout = new QVBoxLayout(this);
    vlayout->setSpacing(5);

    _id_label = new DLabel(this);
    _name_edit = new DLineEdit(this);
    _price_edit = new DLineEdit(this);
    _desc_edit = new DPlainTextEdit(this);

    _save_button = new DSuggestButton(tr("保存"), this);
    _delete_button = new DWarningButton(this);
    _delete_button->setText("删除");
    _name_edit->setPlaceholderText(tr("名称"));
    _price_edit->setPlaceholderText(tr("价格"));
    _desc_edit->setPlaceholderText(tr("描述"));

    _desc_edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    vlayout->addWidget(_delete_button);
    vlayout->addWidget(_id_label);
    vlayout->addWidget(_name_edit);
    vlayout->addWidget(_price_edit);
    vlayout->addWidget(_desc_edit);
    vlayout->addWidget(_save_button);
    _price_edit->lineEdit()->setValidator(
        new QDoubleValidator(0, std::numeric_limits<double>::max(), 2, this));

    // code specified for DDialog
    auto mainContent = new QWidget(this);
    mainContent->setLayout(vlayout);
    addContent(mainContent);

    connect(_delete_button, &DWarningButton::clicked, this, &item_edit_page::on_delete_click);
    connect(_save_button, &DSuggestButton::clicked, this, &item_edit_page::on_save_click);
}

void item_edit_page::update_new()
{
    _delete_button->setVisible(false);
    _delete_button->setEnabled(false);
    _id_label->setVisible(false);
    _name_edit->clear();
    _price_edit->clear();
    _desc_edit->clear();
    _creating_item = true;
}

void item_edit_page::update(core::model::item const& item, bool delete_enabled)
{
    _item_id = item.id();
    _delete_button->setVisible(delete_enabled);
    _delete_button->setEnabled(delete_enabled);
    _id_label->setVisible(true);
    _id_label->setText(QString::fromStdString(_item_id));
    _name_edit->setText(QString::fromStdString(item.name()));
    _price_edit->setText(arg_price_cents("%1", item.price_cents()));
    _desc_edit->setPlainText(QString::fromStdString(item.description()));
    _creating_item = false;
}

void item_edit_page::on_save_click()
{
    if (_name_edit->text().isEmpty() 
        || _price_edit->text().isEmpty() || _desc_edit->toPlainText().isEmpty())
    {
        critial_message_box("所有项均为必填。", "验证失败");
        return;
    }
    auto price_cents = static_cast<int>(std::round(_price_edit->text().toDouble() * 100.0));
    if (_creating_item)
        emit on_create_item(
            _name_edit->text(),
            price_cents,
            _desc_edit->toPlainText()
        );
    else
        emit on_edit_item(
            _item_id, _name_edit->text(),
            price_cents,
            _desc_edit->toPlainText());
}

void item_edit_page::on_delete_click()
{
    auto conf = QMessageBox::question(
        this,
        "删除", tr("确定删除商品 %1(%2) 吗？").arg(_name_edit->text()).arg(_id_label->text()),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (conf != QMessageBox::Yes)
        return;
    emit on_delete_item(_item_id);
}
}
