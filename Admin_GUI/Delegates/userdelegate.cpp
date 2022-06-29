#include "userdelegate.h"
#include <QDebug>

UserDelegate::UserDelegate(QFont &font, QObject *parent)
    : QStyledItemDelegate(parent)
    , m_font(font)
{

}

UserDelegate::~UserDelegate()
{

}

void UserDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //https://doc.qt.io/qt-5/qstyleditemdelegate.html#paint
    if (index.isValid())
    {
        painter->save();

        const QVariant data = index.data(Qt::UserRole + 1);
        const User user = data.value<User>();
        QRect globalRect;
        globalRect.setX(option.rect.x());
        globalRect.setY(option.rect.y());
        globalRect.setWidth(option.rect.width());
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

        QPen titlePen, textPen;
        if (option.state.testFlag(QStyle::State_MouseOver))
        {
            QPen mouseOverPen(option.palette.color(QPalette::ToolTipBase));
            painter->setPen(mouseOverPen);
            painter->setBrush(mouseOverPen.color());
            painter->drawPath(path);
            titlePen.setColor(option.palette.color(QPalette::Link));
            textPen.setColor(option.palette.color(QPalette::Base));
        }
        else
        {
            if (option.state.testFlag(QStyle::State_Selected))
            {
                QPen mouseSelectedPen(option.palette.color(QPalette::Highlight));
                painter->setPen(mouseSelectedPen);
                painter->setBrush(mouseSelectedPen.color());
                painter->drawPath(path);
            }

            titlePen.setColor(option.palette.color(QPalette::HighlightedText));
            textPen.setColor(option.palette.color(QPalette::WindowText));
        }
        QRect iconRect, userIdRect, userNameRect, userFCSRect, userRoleRect;
        iconRect.setRect(globalRect.left() + 5, globalRect.top() + 5, m_font.pointSize() * 4 + 40, m_font.pointSize() * 4 + 40);
        userIdRect.setRect(iconRect.right() + 5, iconRect.top(), globalRect.width() - 10 - iconRect.width(), m_font.pointSize() + 10);
        userNameRect.setRect(userIdRect.left(), userIdRect.bottom() + 1, globalRect.width() - iconRect.width(), m_font.pointSize() + 10);
        userFCSRect.setRect(userIdRect.left(), userNameRect.bottom() - 1, globalRect.width() - iconRect.width(), m_font.pointSize() + 10);
        userRoleRect.setRect(userIdRect.left(), userFCSRect.bottom() - 1, globalRect.width() - iconRect.width(), m_font.pointSize() + 10);

        painter->drawImage(iconRect, QImage(user.GetUserImage()));

        painter->setPen(titlePen);
        painter->setFont(m_font);
        painter->drawText(userIdRect, "Идентификатор: " + user.GetUserId());

        painter->setPen(textPen);
        painter->drawText(userNameRect, "Имя пользователя: " + user.GetUserName());
        painter->drawText(userFCSRect, "ФИО: " + user.GetUserFCS());
        const QString role = (user.GetUserRole() >= 0 && user.GetUserRole() < Roles.count()) ? Roles.at(user.GetUserRole()) : " ";
        painter->drawText(userRoleRect, "Роль: " + role);
        painter->restore();

    }
}

QSize UserDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    Q_UNUSED(option)
    QSize size(150, m_font.pointSize() * 4 + 50);
    return size;
}
