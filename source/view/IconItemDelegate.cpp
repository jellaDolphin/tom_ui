#include "IconItemDelegate.h"

#include <QPainter>
#include "FrameTableView.h"

IconItemDelegate::IconItemDelegate(const QIcon &icon, FrameTableView *parent) : QStyledItemDelegate(parent) {
    _icon = icon;
    _view = parent;
}

void IconItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (!index.isValid()) {
        return;
    }

    // paint the defaults, e.g. the selection background
    QStyledItemDelegate::paint(painter, option, QModelIndex());

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    auto opt = option;
    QStyledItemDelegate::initStyleOption(&opt, index);

    if (_view->isPersistentEditorOpen(index)) {
        painter->fillRect(option.rect, Qt::GlobalColor::white);
    } else if (opt.features.testFlag(QStyleOptionViewItem::ViewItemFeature::HasDisplay) && index.data(Qt::EditRole).toBool()) {
        QRect r = option.rect;
        int w = r.width();
        int h = r.height();
        if (w > 16 && h > 16) {
            r.setWidth(16);
            r.setHeight(16);
            r.translate((w - 16) / 2, (h - 16) / 2);
        }

        _icon.paint(painter, r, Qt::AlignCenter, QIcon::Disabled);
    }

    painter->restore();
}
