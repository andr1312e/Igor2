#include "filedelegate.h"

FileDelegate::FileDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void FileDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid())
    {
       painter->save();

       QVariant data=index.data(Qt::UserRole+1);
       DesktopEntity icon=data.value<DesktopEntity>();

       QRect rect;
       rect.setX(option.rect.x());
       rect.setY(option.rect.y());

       rect.setWidth(option.rect.width()+1);
       rect.setHeight(option.rect.height()-1);

       QPainterPath path;
       path.moveTo(rect.topRight());
       path.lineTo(rect.topLeft());
       path.quadTo(rect.topLeft(), rect.topLeft());
       path.lineTo(rect.bottomLeft());
       path.quadTo(rect.bottomLeft(), rect.bottomLeft());
       path.lineTo(rect.bottomRight());
       path.quadTo(rect.bottomRight(), rect.bottomRight());
       path.lineTo(rect.topRight());
       path.quadTo(rect.topRight(), rect.topRight());

       if (option.state.testFlag(QStyle::State_MouseOver)) {
           painter->setPen(QPen(QColor("#ebeced")));
           painter->setBrush(QColor("#ebeced"));
           painter->drawPath(path);
       }
       if (option.state.testFlag(QStyle::State_Selected)) {
           painter->setPen(QPen(QColor("#e3e3e5")));
           painter->setBrush(QColor("#e3e3e5"));
           painter->drawPath(path);
       }
       QRect iconRect = QRect(rect.left()+5, rect.top()+5, 45, 45);
       QRect userIdRect = QRect(iconRect.right()+5, iconRect.top(), rect.width()-10-iconRect.width(), 20);
       QRect userNameRect = QRect(userIdRect.left(), userIdRect.bottom()+1, rect.width()-iconRect.width(), 20);
       QRect userFCSRect = QRect(userIdRect.left(), userNameRect.bottom()-1, rect.width()-iconRect.width(), 20);

//       painter->drawImage(iconRect, QImage(program.m_image));
       painter->setPen(QPen(Qt::white));
       painter->setFont(QFont("Microsoft Yahei", 12));
       painter->drawText(userIdRect, "Название: " + icon.name);

       painter->setPen(QPen(Qt::gray));
       painter->drawText(userNameRect, "Путь: " + icon.exec);
       painter->drawText(userFCSRect, "Тип: " + icon.type);



       painter->restore();
    }
}

QSize FileDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(), 70);
}
