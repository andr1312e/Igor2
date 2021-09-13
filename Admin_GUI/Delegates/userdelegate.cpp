#include "userdelegate.h"
#include <QDebug>

UserDelegate::UserDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void UserDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //https://doc.qt.io/qt-5/qstyleditemdelegate.html#paint
    if (index.isValid())
    {
        painter->save();

        QVariant data=index.data(Qt::UserRole+1);
        User user=data.value<User>();

        QRect rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
//        qDebug()<< "width" << option.rect.width() <<  " height   " << option.rect.height();
//        qDebug()<< "rect " << rect;
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

        QRect iconRect = QRect(rect.left()+5, rect.top()+5, 65, 65);
        QRect userIdRect = QRect(iconRect.right()+5, iconRect.top(), rect.width()-10-iconRect.width(), 20);
        QRect userNameRect = QRect(userIdRect.left(), userIdRect.bottom()+1, rect.width()-iconRect.width(), 20);
        QRect userFCSRect = QRect(userIdRect.left(), userNameRect.bottom()-1, rect.width()-iconRect.width(), 20);
        QRect userRoleRect = QRect(userIdRect.left(), userFCSRect.bottom()-1, rect.width()-iconRect.width(), 20);

        painter->drawImage(iconRect, QImage(user.m_image));
        painter->setPen(QPen(Qt::white));
        painter->setFont(QFont("Microsoft Yahei", 12));
        painter->drawText(userIdRect, "Идентификатор: " + user.userId);

        painter->setPen(QPen(Qt::gray));
        painter->drawText(userNameRect, "Имя пользователя: " + user.name);
        painter->drawText(userFCSRect, "ФИО: " + user.FCS);
        painter->drawText(userRoleRect, "Роль: " + user.role);

        painter->restore();
    }
}

QSize UserDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    qDebug()<< "option.rect.width();" << option.rect.width();
    return QSize(300, 90);
}
