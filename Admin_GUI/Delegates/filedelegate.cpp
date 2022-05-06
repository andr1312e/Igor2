#include "filedelegate.h"
#include <QDebug>

FileDelegate::FileDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

FileDelegate::~FileDelegate()
{
}

void FileDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        QPen textPen, titlePen, mouseOverPen, mouseSelectedPen;
        painter->save();

        QVariant data = index.data(Qt::UserRole + 1);
        DesktopEntity icon = data.value<DesktopEntity>();
        QRect globalRect;
        QRect iconRect;
        QRect programNameRect;
        QRect pathRect;
        QRect typeRect;
        globalRect.setX(option.rect.x());
        globalRect.setY(option.rect.y());

        globalRect.setWidth(option.rect.width() + 1);
        globalRect.setHeight(option.rect.height() - 1);

        QPainterPath path;
        path.moveTo(globalRect.topRight());
        path.lineTo(globalRect.topLeft());
        path.quadTo(globalRect.topLeft(), globalRect.topLeft());
        path.lineTo(globalRect.bottomLeft());
        path.quadTo(globalRect.bottomLeft(), globalRect.bottomLeft());
        path.lineTo(globalRect.bottomRight());
        path.quadTo(globalRect.bottomRight(), globalRect.bottomRight());
        path.lineTo(globalRect.topRight());
        path.quadTo(globalRect.topRight(), globalRect.topRight());

        if (option.state.testFlag(QStyle::State_MouseOver)) {
            mouseOverPen.setColor(option.palette.color(QPalette::ToolTipBase));
            painter->setPen(mouseOverPen);
            painter->setBrush(mouseOverPen.color());
            painter->drawPath(path);
            titlePen.setColor(option.palette.color(QPalette::Link));
            textPen.setColor(option.palette.color(QPalette::Base));
        } else {
            if (option.state.testFlag(QStyle::State_Selected)) {
                mouseSelectedPen.setColor(option.palette.color(QPalette::Highlight));
                painter->setPen(mouseSelectedPen);
                painter->setBrush(mouseSelectedPen.color());
                painter->drawPath(path);
            }

            titlePen.setColor(option.palette.color(QPalette::HighlightedText));
            textPen.setColor(option.palette.color(QPalette::WindowText));
        }

        iconRect.setRect(globalRect.left() + 5, globalRect.top() + 5, 45, 45);
        programNameRect.setRect(iconRect.right() + 5, iconRect.top(), globalRect.width() - 10 - iconRect.width(), 20);
        pathRect.setRect(programNameRect.left(), programNameRect.bottom() + 1, globalRect.width() - iconRect.width(), 20);
        typeRect.setRect(programNameRect.left(), pathRect.bottom() - 1, globalRect.width() - iconRect.width(), 20);

        painter->drawImage(iconRect, QImage(icon.icon));

        painter->setPen(titlePen);
        painter->setFont(QFont(QStringLiteral("Microsoft Yahei"), 12));
        painter->drawText(programNameRect, "Название: " + icon.name);

        painter->setPen(textPen);
        painter->drawText(pathRect, "Путь: " + icon.exec);
        painter->drawText(typeRect, "Тип: " + icon.type);
        painter->restore();
    }
}

QSize FileDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size(option.rect.width(), 70);
    Q_UNUSED(index)
    return size;
}
