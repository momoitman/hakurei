#include "item_list_delegate.h"

#include <QPainter>
#include <date/date.h>

#include "item_list_model.h"

namespace hakurei::ui
{
void item_list_delegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    // 人生苦短，别用 Qt
    auto model = qobject_cast<model::item_list_model const*>(index.model());
    auto item = model->get_item(index.row());
    const QPalette& palette = option.palette;

    // paint background
    QBrush brush;
    auto font = option.font;
    auto rect = option.rect;
    int bx = rect.x();
    int by = rect.y();
    font.setPixelSize(24);
    painter->setFont(font);
    
    painter->setBrush(option.state & QStyle::State_Selected ? palette.highlight() : palette.light());
    auto background_rect = option.rect;
    background_rect.adjust(3, 3, -3, -3);
    painter->drawRoundedRect(background_rect, 3.0, 3.0);

    painter->translate(bx, by);

    painter->setPen(option.state & QStyle::State_Selected ? palette.highlightedText().color() : palette.text().color());
    auto top_str = QString::fromStdString(date::format("%F", item->on_stock_time()) + " · " + item->seller_uid());
    font.setPixelSize(12);
    painter->setFont(font);
    auto top_str_br = QFontMetrics(font).boundingRect(top_str);
    top_str_br.moveTo(30, 15);
    painter->drawText(top_str_br, top_str);
    
    auto price_str = QString::asprintf("%.02f", item->price_cents() / 100.0);
    
    font.setPixelSize(40);
    auto price_str_br = QFontMetrics(font).boundingRect(price_str);
    
    font.setPixelSize(53);
    auto price_mark_br = QFontMetrics(font).boundingRect("￥");
    int rptr = rect.width() - 30 - price_str_br.width() - price_mark_br.width();
    
    price_mark_br.moveTo(rptr - 10, 7);
    price_str_br.moveTo(rptr + price_mark_br.width(), 22);
    
    font.setPixelSize(53);
    painter->setFont(font);
    painter->drawText(price_mark_br.adjusted(-5, 0, 5, 0), "￥");
    
    font.setPixelSize(40);
    painter->setFont(font);
    painter->drawText(price_str_br.adjusted(-5, 0, 5, 10), price_str);
    
    QFontMetrics fm(font);
    font.setPixelSize(24);
    painter->setFont(font);
    auto name_str = fm.elidedText(QString::fromStdString(item->name()), Qt::ElideRight, rptr - 30);
    auto name_str_br = fm.boundingRect(name_str);
    name_str_br.moveTo(30, 27);
    painter->drawText(name_str_br, name_str);
    
    font.setPixelSize(16);
    fm = QFontMetrics(font);
    painter->setFont(font);
    auto desc_str = fm.elidedText(QString::fromStdString(item->description()), Qt::ElideRight, rptr - 30);
    auto desc_str_br = fm.boundingRect(desc_str);
    desc_str_br.moveTo(30, 57);
    painter->drawText(desc_str_br, desc_str);

    painter->resetTransform();
}

QSize item_list_delegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return QSize(option.rect.width(), 90);
}
}
